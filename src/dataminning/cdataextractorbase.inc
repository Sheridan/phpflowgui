<?php
abstract class CDataExtractorBase
{
	protected $data_parced;
	function __construct()
	{}

	function __destruct()
	{}

	private function file_strings_count($file_name)
	{
		return exec("cat {$file_name} | wc -l") - 1;
    }

	private function store_text($text, $prefix)
	{
		global $config;
		$filename = tempnam($config['temporally_directory'], "pfg-{$prefix}-");
		file_put_contents($filename, $text);
		return $filename;
    }
	
	protected function execute_extract($command_line, $filters_text, $report_text, $direction, $key_field)
	{
		global $page;
		global $config;
		$page->set_progress_text('Extracting flow data');
		$output_filename = tempnam($config['temporally_directory'], "pfg-report-outputdata-");
		$filter_filename = $this->store_text($filters_text, 'filters');
		$report_text = str_replace('filter_filename', $filter_filename, $report_text);
		$report_text = str_replace('output_filename', $output_filename, $report_text);
		$report_filename = $this->store_text($report_text, 'reports');
		$command_line = str_replace('report_filename', $report_filename, $command_line);
		exec($command_line);
		unlink($filter_filename);
		unlink($report_filename);
		/*
		file_put_contents("d_filter", $filters_text);
		file_put_contents("d_report", str_replace($filter_filename, 'd_filter', $report_text));
		file_put_contents("d_run", str_replace($report_filename, 'd_report', $command_line) . "\ncat {$output_filename}\n rm -f {$output_filename}");
		chmod("d_run", 0755);
		*/
		$this->append_data($output_filename, $direction, $key_field);
    }
	
	private function append_data($output_filename, $direction, $key_field)
	{
		global $page;
		$key = '';
		$octets = 0;
		$page->progressbar_init("Parsing flow data ({$direction})", $this->file_strings_count($output_filename));
		$output_handle = fopen($output_filename, "r");
		while (!feof($output_handle))
		{
			$line = rtrim(fgets($output_handle), "\n");
			if(preg_match('/^(\s*?(#|\s)|\s*$)/', $line)) { continue; }
			list($key, $octets) = split(',', $line);
			if(!isset($this->data_parced['i'][$key]))
			{
				$this->data_parced['i'][$key] = count($this->data_parced[$key_field]);
			}
			$index = $this->data_parced['i'][$key];
			$this->data_parced[$key_field][$index] = $key;
			$this->data_parced[$direction][$index] = $octets;
			$page->progressbar_step();
        }
		fclose($output_handle);
		unlink($output_filename);
    }
	
	protected function get_report_base_text($report_type)
	{
		return <<<_

include-filter filter_filename

stat-report report_primitive
	type {$report_type}
	filter fd
	output
		format ascii
		options -header,-xheader,-totals,-names
		fields +key,+octets,-flows,-packets,-duration
		path output_filename

stat-definition this_report
	report report_primitive

_;
    }
}
?>
