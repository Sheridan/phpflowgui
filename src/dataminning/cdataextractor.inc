<?php
require "cdataextractorbase.inc";

class CDataExtractor extends CDataExtractorBase
{
    function __construct()
    {
		parent::__construct();
    }

    function __destruct()
    {
		parent::__destruct();
    }

	private function build_filter_part($filter_name, $filter_type, $s_elements, $d_elements, $default_access)
	{
		$result = '';
		$s_count = isset($s_elements['items']) ? count($s_elements['items']) : 0;
		$d_count = isset($d_elements['items']) ? count($d_elements['items']) : 0;
		$max_count = max_int($s_count, $d_count);
		//dprint ('max count', $max_count);
		for($i = 0; $i < $max_count; $i++)
		{
			if($i < $s_count)
			{
				$access = $s_elements['invert'] ? 'deny' : 'permit';
            	if($access != $default_access) { $result .= "\t{$access} {$s_elements['items'][$i]}\n"; }
			}
			if($i < $d_count)
			{
				$access = $d_elements['invert'] ? 'permit' : 'deny';
				if($access != $default_access) { $result .= "\t{$access} {$d_elements['items'][$i]}\n"; }
			}
        }
		//dprint ('result', $result);
		if(strlen($result))
		{
			return <<<_

filter-primitive {$filter_name}
	type {$filter_type}
{$result}	default {$default_access}
_;
        }
		return '';
    }

	private function build_filters_text(&$filter)
	{
		dprint('raw filter', $filter);
		$filters = array();
		$filter_primitives = array();
		$filter_primitive_name = array();
		$need_to_mirror = false;
		foreach($filter as $name => $f_values)
		{
			$filter_primitive_type = '';
			$filter_definition_match = '';
			
			switch($name)
			{
				case 'network':
				{
					$filter_primitive_type = 'ip-address-prefix';
					$filter_definition_match = 'ip-[direction]-address';
					$need_to_mirror = true;
                }
				break;
				case 'host':
				{
					$filter_primitive_type = 'ip-address';
					$filter_definition_match = 'ip-[direction]-address';
					$need_to_mirror = true;
                }
				break;
				case 'port':
				{
					$filter_primitive_type = 'ip-port';
					$filter_definition_match = 'ip-[direction]-port';
					$need_to_mirror = false;
                }
				break;
				case 'protocol': 
				{
					$filter_primitive_type = 'ip-protocol';
					$filter_definition_match = 'ip-protocol';
					$need_to_mirror = false;
                }
				break;
				case 'tos':
				{
					$filter_primitive_type = 'ip-tos';
					$filter_definition_match = 'ip-tos';
					$need_to_mirror = false;
                }
				break;
				case 'as':
				{
					$filter_primitive_type = 'as';
					$filter_definition_match = '[direction]-as';
					$need_to_mirror = false;
                }
				break;
            }
			
			$filter_primitive_name[0] = "filter_primitive_{$name}_a";
			$filter_primitives[0] = $this->build_filter_part($filter_primitive_name[0], $filter_primitive_type, $f_values['destination'], $f_values['source'], ($f_values['destination']['invert'] ? 'permit' : 'deny'));
			if(strlen($filter_primitives[0]))
			{
				$filters['destination']['primitives'] .=  $filter_primitives[0];
				$filters['destination']['definitions'] .=  "\tmatch " . str_replace('[direction]', 'destination', $filter_definition_match) . " " . $filter_primitive_name[0] . "\n";
				$filters['source']['primitives'] .=  $filter_primitives[0];
				$filters['source']['definitions'] .=  "\tmatch " . str_replace('[direction]', 'source', $filter_definition_match) . " " . $filter_primitive_name[0] . "\n";
			}
			if($need_to_mirror)
			{
				$filter_primitive_name[1] = "filter_primitive_{$name}_b";
				$filter_primitives[1] = $this->build_filter_part($filter_primitive_name[1], $filter_primitive_type, $f_values['source'], $f_values['destination'], ($f_values['source']['invert'] ? 'deny' : 'permit'));
				if(strlen($filter_primitives[1]))
				{
					$filters['destination']['primitives'] .=  $filter_primitives[1];
					$filters['destination']['definitions'] .= "\tmatch " . str_replace('[direction]', 'source', $filter_definition_match) . " " . $filter_primitive_name[1] . "\n";
					$filters['source']['primitives'] .=  $filter_primitives[1];
					$filters['source']['definitions'] .=  "\tmatch " . str_replace('[direction]', 'destination', $filter_definition_match) . " " . $filter_primitive_name[1] . "\n";
				}
			}
			
		}
		$result = array();
		foreach(array('source', 'destination') as $direction)
		{
			$result[$direction] = <<<_
{$filters[$direction]['primitives']}
filter-definition fd
{$filters[$direction]['definitions']}
_;
		}
		dprint('Filters', &$result);
		return $result;
    }
	
	private function build_reports_text()
	{
		global $sm;
		$reports = array();
		foreach(array('source', 'destination') as $direction)
		{
			$report_type = '';
			switch($sm->value('view'))
			{
				case 'protocols': $report_type = "ip-protocol"; break;
				case 'tos': $report_type = "ip-tos"; break;
				case 'as': $report_type = "{$direction}-as"; break;
				case 'hosts': $report_type = "ip-{$direction}-address"; break;
				case 'ports': $report_type = "ip-{$direction}-port"; break;
            }
			$reports[$direction] = $this->get_report_base_text($report_type);
        }
		return $reports;
    }

    public function &get_traffic(&$filter, &$date, $sorting, $key_field, $name_cache)
    {
		global $config;
		global $page;
		global $sm;
		/*$cache_filename = sprintf("%s_%s_%s_%s_%s",
									$date['year'],
									$date['month'] != '' ? $date['month'] : 'a',
									$date['day'] != '' ? $date['day'] : 'a',
									$key_field,
									$sorting['sort_field']
							);*/
		$filters = $this->build_filters_text($filter);
		$reports = $this->build_reports_text();

//		dprint('Networks', $networks);
//		dprint('Filters', $filters);
//		dprint('Reports', $reports);
		$flow_files = "{$config['flow']['filename_prefix']}{$date['year']}-" . ($date['month'] != '' ? "{$date['month']}-" . ($date['day'] != '' ? "{$date['day']}*" : "*") : "*");
		$cmd = "flow-cat {$config['flow']['data_path']}/{$flow_files} | flow-report -sreport_filename -Sthis_report";
		//dprint('Command', $cmd);
		$result = array();
		$this->data_parced = array($key_field => array(), 'in' => array(), 'out' => array(), 'i' => array(), 'name' => array());
		$this->execute_extract($cmd, $filters['destination'], $reports['destination'], 'in', $key_field);
		$this->execute_extract($cmd, $filters['source'], $reports['source'], 'out', $key_field);
//		dprint('Data', &$data_parts);
		if($name_cache)
		{
			$page->progressbar_init('Resolving names', count($this->data_parced[$key_field]));
			$cnt = count($this->data_parced[$key_field]);
			for($fi = 0; $fi < $cnt; $fi++)
			{
				$this->data_parced['name'][$fi] = $sm->value('show_all_names') 
						? $name_cache->values_str($this->data_parced[$key_field][$fi])
						: $name_cache->value($this->data_parced[$key_field][$fi]);
				$page->progressbar_step();
            }
        }
		$page->set_progress_text('Sorting...');
		$sort_funct = ($sorting['sort_order'] == 'asc' ? 'cmp_' : 'rcmp_') . (($sorting['sort_field'] == $key_field) ? ($name_cache ? $sorting['sort_name_type'] : $sorting['sort_data_type']) : 'int');
		$sort_field = ($sorting['sort_field'] == $key_field && $name_cache) ? 'name' : $sorting['sort_field'];
		//echo "$sort_funct - $sort_field";
		uasort(&$this->data_parced[$sort_field], $sort_funct);
		$page->progressbar_init('Rebuilding flow data array', count($this->data_parced[$sort_field]));
		foreach($this->data_parced[$sort_field] as $index => $value)
		{
			$result[] = array	(
									$key_field => $this->data_parced[$key_field][$index],
									'name' => $name_cache ? $this->data_parced['name'][$index] : $this->data_parced[$key_field][$index],
									'in' => isset($this->data_parced['in'][$index]) ? $this->data_parced['in'][$index] : 0,
									'out' => isset($this->data_parced['out'][$index]) ? $this->data_parced['out'][$index] : 0
								);
			unset($this->data_parced[$key_field][$index]);
			unset($this->data_parced['name'][$index]);
			unset($this->data_parced['in'][$index]);
			unset($this->data_parced['out'][$index]);
			$page->progressbar_step();
        }
		$page->set_progress_text('Extract done');
		//dprint('Data', &$result);
		return $result;
    }
}
?>
