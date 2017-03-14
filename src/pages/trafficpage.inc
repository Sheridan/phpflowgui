<?php
include_once('cpage.inc');
include_once('src/dataminning/cdataextractor.inc');

class TrafficPage extends CPage
{
	private $filter;
	
	function __construct()
    {
		global $sm;
		parent::__construct();
		$sm->prepare_variable('view', TSM_STRING, 'hosts', true, TSM_GET);
		$this->show_options_form();
		$this->prepare_filter();
    }
	
    function __destruct()
    {
		parent::__destruct();
    }

	private function show_options_form()
	{
		global $config;
		global $sm;
		global $data_types;
		$checked = null;
		$this->show_window_table_header('fopt', 'Filter options');
		$js = "";
		echo <<<_
<form method='post' name='fopt' id='id_fopt' action='index.php' onsubmit='fopt_submit();'>
<table align='center'>
<colgroup>
	<col width='33%' style='min-width: 200px;' />
	<col width='33%' style='min-width: 200px;' />
	<col width='34%' style='min-width: 200px;' />
</colgroup>
<tr>
	<th class='up'>Source</th>
	<th class='up'>Destination</th>
	<th class='up'>Filter</th>
</tr>
_;
		foreach($data_types as $name => $values)
		{
			echo <<<_
<tr>
	<th colspan='3'>{$values[TT_DATA_CAPTION]}</th>
</tr>
<tr>
_;
			foreach(array('source', 'destination') as $direction)
			{
				$id = "{$direction}_{$name}";
				$sm->prepare_variable("invert_{$id}", TSM_BOOL, false);
				$checked = $sm->value("invert_{$id}") ? array('h' => 'on', 'c' => 'checked') : array('h' => 'off', 'c' => '');
				echo <<<_
	<td>
		<table align='center' class='unvisible'>
		<colgroup>
			<col width='90%' />
			<col width='5%' />
		</colgroup>
		<tr class='unvisible'>
			<td class='unvisible'>
				<input type='checkbox' id='id_checkbox_invert_{$id}' name='cb_invert_{$id}' onchange="set_checkbox_changed('invert_{$id}')" {$checked['c']} />All but selected<br />
				<input type='hidden' id='id_hidden_invert_{$id}' name='invert_{$id}' value='{$checked['h']}' />
				<select multiple='multiple' size='4' name='select_{$id}' id='id_select_{$id}'></select>
				<input type='hidden' name='{$id}' id='id_hidden_{$id}' value='' />
			</td>
			<td class='unvisible'>
				<img src='oimg.php?i=t_{$name}&amp;o=remove' title='Remove selected' onclick="remove_from_select('id_select_{$id}');" />
				<img src='oimg.php?i=t_{$name}&amp;o=clear' title='Remove selected' onclick="remove_all_from_select('id_select_{$id}');" />
			</td>
		</tr>
		</table>
	</td>
_;
				$js .= "copy_select_to_hidden('id_select_{$id}', 'id_hidden_{$id}');";
            }
			echo '<td>';
			$advanced_btt = '&nbsp;';
			if(is_array($config['filter'][$name]) && count($config['filter'][$name]))
			{
				$table_id = $this->show_window_table_header("fopt{$name}", "{$values[TT_DATA_CAPTION]} filter options", false);
				$advanced_btt = "<img src='img/options.png' alt='{$values[TT_DATA_CAPTION]} filter options' onclick=\"hu_open('{$table_id}')\" />";
				echo <<<_
<table align='center'>
<colgroup>
	<col width='5%' />
	<col width='45%' />
	<col width='45%' />
	<col width='5%' />
</colgroup>
_;
				foreach($config['filter'][$name] as $data)
				{
					echo <<<_
	<tr>
		<td><img src='oimg.php?i=t_{$name}&amp;o=arr_from' title='Append as source' onclick="append_to_select('id_select_source_{$name}','{$data['data']}');" /></td>
		<td align='right'>{$data['name']}</td>
		<td align='left'>{$data['data']}</td>
		<td><img src='oimg.php?i=t_{$name}&amp;o=arr_to' title='Append as destination' onclick="append_to_select('id_select_destination_{$name}','{$data['data']}');" /></td>
	</tr>
_;
				}
				echo '</table>';
				$this->show_window_table_footer();
			}
			echo <<<_
	<table align='center'>
	<colgroup>
		<col width='5%' />
		<col width='85%' />
		<col width='5%' />
		<col width='5%' />
	</colgroup>
	<tr class='unvisible'>
		<td><img src='oimg.php?i=t_{$name}&amp;o=arr_from' title='Append as source' onclick="append_to_select_from_edit('id_select_source_{$name}','id_edit_{$name}');" /></td>
		<td align='center'><input type='text' id='id_edit_{$name}' name='edit_{$name}' style='width: 100%;' /></td>
		<td><img src='oimg.php?i=t_{$name}&amp;o=arr_to' title='Append as destination' onclick="append_to_select_from_edit('id_select_destination_{$name}','id_edit_{$name}');" /></td>
		<td class='unvisible'>{$advanced_btt}</td>
	</tr>
	</table>
</td>
</tr>
_;
        }
		echo <<<_
<tr>
	<th class='up'>Source</th>
	<th class='up'>Destination</th>
	<th class='up'>Filter</th>
</tr>
<tr><th class='up' colspan='3'><input type='submit' value='Build' /></th></tr>
</table>
<script type='text/javascript'>
//<![CDATA[
function fopt_submit()
{
	{$js}
	fopt.submit();
}
//]]>
</script>
</form>
_;
		$this->show_window_table_footer();
		
		$this->show_window_table_header('opt', 'View options');
		$sm->prepare_variable_batch(array('show_as_names', 'show_all_names', 'show_only_names'), TSM_BOOL, array(false, false, false));
		$checked['as']	= $sm->value('show_as_names') ? array('h' => 'on', 'c' => 'checked') : array('h' => 'off', 'c' => '');
		$checked['all']	= $sm->value('show_all_names') ? array('h' => 'on', 'c' => 'checked') : array('h' => 'off', 'c' => '');
		$checked['on']	= $sm->value('show_only_names') ? array('h' => 'on', 'c' => 'checked') : array('h' => 'off', 'c' => '');
		echo <<<_
<form method='post' name='opt' action='index.php'>
<table align='center'>
<tr>
	<td>
		<table align='center'>
		<tr><td><input type='checkbox' id='id_checkbox_show_as_names' name='cb_show_as_names' onchange="set_checkbox_changed('show_as_names')" {$checked['as']['c']} /></td><td>Show as names</td></tr>
		<tr><td><input type='checkbox' id='id_checkbox_show_all_names' name='cb_show_all_names' onchange="set_checkbox_changed('show_all_names')" {$checked['all']['c']} /></td><td>Show all names</td></tr>
		<tr><td><input type='checkbox' id='id_checkbox_show_only_names' name='cb_show_only_names' onchange="set_checkbox_changed('show_only_names')" {$checked['on']['c']} /></td><td>Show only names</td></tr>
		</table>
	</td>
</tr>
<tr><th class='up' colspan='2'><input type='submit' value='Set' /></th></tr>
</table>
<input type='hidden' id='id_hidden_show_as_names' name='show_as_names' value='{$checked['as']['h']}' />
<input type='hidden' id='id_hidden_show_all_names' name='show_all_names' value='{$checked['all']['h']}' />
<input type='hidden' id='id_hidden_show_only_names' name='show_only_names' value='{$checked['on']['h']}' />
</form>
_;
		$this->show_window_table_footer();
    }
	
	private function prepare_filter()
	{
		global $sm;
		global $data_types;
		foreach($data_types as $name => $values)
		{
			foreach(array('source', 'destination') as $direction)
			{
				$id = "{$direction}_{$name}";
				$sm->prepare_array($id, $values[TT_DATA_TYPE]);
				$f_value = $sm->value($id);
				//dprint("gv_{$id}[{$values[TT_DATA_TYPE]}]", &$f_value);
				//Сразу добавляем в спиcки фильтров
				if($f_value != null)
				{
					$this->filter[$name][$direction]['items'] = $f_value;
					$this->filter[$name][$direction]['invert'] = $sm->value("invert_{$id}");
					foreach($this->filter[$name][$direction]['items'] as $data)
					{
						echo <<<_
<script type='text/javascript'>
//<![CDATA[
append_to_select('id_select_{$id}','{$data}');
//]]>
</script>
_;
					}
				}
				
			}
        }
		//dprint('Filters', $this->filter);
    }
	
	public function show()
    {
		global $config;
		global $sm;
		global $data_types;
		$extractor = new CDataExtractor();
		$view_data = array();
		switch($sm->value('view'))
		{
			case 'protocols':
			{
				include_once('src/cache/protocolcache.inc');
				$view_data = array	(
									'name'		=> 'protocol',
									'data_type'	=> 'int',
									'name_type'	=> 'strint',
									'caption'	=> $sm->value('show_as_names') ? $data_types['protocol'][TT_LABEL_CAPTION] : $data_types['protocol'][TT_DATA_CAPTION],
									'cache'		=> $sm->value('show_as_names') ? new ProtocolCache() : null
									);
			}
			break;
			case 'hosts':
			{
				include_once('src/cache/dnscache.inc');
				$view_data = array	(
									'name'		=> 'host',
									'data_type'	=> 'ip',
									'name_type'	=> 'hostname',
									'caption'	=> $sm->value('show_as_names') ? $data_types['host'][TT_LABEL_CAPTION] : $data_types['host'][TT_DATA_CAPTION],
									'cache'		=> $sm->value('show_as_names') ? new DNSCache() : null
									);
			}
			break;
			case 'ports':
			{
				include_once('src/cache/portcache.inc');
				$view_data = array	(
									'name'		=> 'port',
									'data_type'	=> 'int',
									'name_type'	=> 'strint',
									'caption'	=> $sm->value('show_as_names') ? $data_types['port'][TT_LABEL_CAPTION] : $data_types['port'][TT_DATA_CAPTION],
									'cache'		=> $sm->value('show_as_names') ? new PortCache() : null
									);
			}
			break;
			case 'tos':
			{
				$view_data = array	(
									'name'		=> 'tos',
									'data_type'	=> 'int',
									'name_type'	=> 'int',
									'caption'	=> $data_types['tos'][TT_DATA_CAPTION],
									'cache'		=> null
									);
			}
			break;
			case 'as':
			{
				include_once('src/cache/ascache.inc');
				$view_data = array	(
									'name'		=> 'as',
									'data_type'	=> 'int',
									'name_type'	=> 'strint',
									'caption'	=> $data_types['as'][TT_DATA_CAPTION],
									'cache'		=> $sm->value('show_as_names') ? new AsCache() : null
									);
			}
			break;
        }
		$header = $this->get_header_data(array($view_data['name'] => $view_data['caption'], 'in' => 'Incoming traffic', 'out' => 'Outgoing traffic'), $view_data['name']);
		$data = $extractor->get_traffic	(
				$this->filter,
				$this->current_date,
				array(	'sort_field' => $header['sort_field'],
						'sort_order' => $header['sort_order'],
						'sort_data_type'  => $view_data['data_type'],
						'sort_name_type'  => $view_data['name_type']),
				$view_data['name'],
				$view_data['cache']
										);
		echo <<<_
<br />
<table align='center' class='outer'>
<tr>
	{$header['table_header']}
</tr>
_;
		$tcount = 0;
		$tin = 0;
		$tout = 0;
		$this->progressbar_init('Output...', count($data));
		foreach($data as $values)
		{
			$tcount++;
			$tin += $values['in'];
			$tout += $values['out'];
			if(!($sm->value('show_as_names') && $sm->value('show_only_names') && $values[$view_data['name']] == $values['name']))
			{
				$oi = num_format($values['in'], 'b');
				$oo = num_format($values['out'], 'b');
				$data_value = $this->convert_data_value($values[$view_data['name']]);
				//dprint('view', $values);
				$hostname_img = $values[$view_data['name']] == $values['name'] ? '' : "<img src='img/name.png' alt='{$data_value['caption']}' />&nbsp;";
				echo <<<_
<tr>
	<td>
		<img src='img/t_{$view_data['name']}.png' alt='{$view_data['name']}' />&nbsp;{$hostname_img}
		<a href='index.php?source_{$view_data['name']}={$data_value}' title='Append as source'><img src='oimg.php?i=t_{$view_data['name']}&amp;o=arr_from' alt='Append as source' /></a>&nbsp
		<a href='index.php?destination_{$view_data['name']}={$data_value}' title='Append as destination'><img src='oimg.php?i=t_{$view_data['name']}&amp;o=arr_to' alt='Append as destination' /></a>&nbsp
		{$values['name']}
		
	</td>
	<td>{$oi}</td>
	<td>{$oo}</td>
</tr>
_;
			}
			$this->progressbar_step();
		}
		//$dnscache = null;
		echo $this->prepare_total(array($tcount, num_format($tin, 'b'), num_format($tout, 'b'))) . '</table>';
		$extractor = null;
    }
}

?>
