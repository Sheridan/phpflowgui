<?php
class CDateTimeExtractor
{
    private $dates; //[years][months][days][hours][minutes]
    function __construct()
    {
		global $config;
		$flow_file_prefix_length = strlen($config['flow']['filename_prefix']);
		$dh  = opendir($config['flow']['data_path']);
		while (false !== ($filename = readdir($dh)))
		{
			if(substr($filename, 0, $flow_file_prefix_length) == $config['flow']['filename_prefix'])
			{
				//ft-v05.2008-09-29.135001+0400
				$this->dates
				[substr($filename, $flow_file_prefix_length,4)]
				[substr($filename, $flow_file_prefix_length+5,2)]
				[substr($filename, $flow_file_prefix_length+8,2)]
				[substr($filename, $flow_file_prefix_length+11,2)]
				[substr($filename, $flow_file_prefix_length+13,2)] = 1;
				//echo $filename . '<br />';
			}
        }
		closedir($dh);
    }

    function __destruct()
    {}

    private function sortedkeys($array)
    {
		if(is_array($array))
		{
		    $array = array_keys($array);
		    sort($array, SORT_NUMERIC);
		}
		return $array;
    }
    
    public function get_years()
    {
		return $this->sortedkeys($this->dates);
    }
    
    public function get_months($year)
    {
		return $this->sortedkeys($this->dates[$year]);
    }

    public function get_days($year, $month)
    {
		return $this->sortedkeys($this->dates[$year][$month]);
    }
	
    public function get_hours($year, $month, $day)
    {
		return $this->sortedkeys($this->dates[$year][$month][$day]);
    }
    //put your code here
}
?>
