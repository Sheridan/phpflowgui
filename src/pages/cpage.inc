<?php
include_once('src/dataminning/cdatetimeextractor.inc');
abstract class CPage
{
    protected $current_date;
	protected $page_type;
	private $page_time;
	private $progress;
	
    function __construct()
    {
		$this->page_time = $this->getmicrotime();
		global $sm;
		$sm->prepare_variable('dt', TSM_SDATE, date('Y_m_d'), true, TSM_GET);
		$this->current_date = $sm->value('dt');
		$this->show_header();
    }
	
    function __destruct()
    {
		$this->show_footer();
    }

    private function show_header()
    {
		global $config;
        echo <<<_
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html>
    <head>
        <meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
		<link rel="stylesheet" type="text/css" href="styles/default/style.css" />
		<link rel="stylesheet" type="text/css" href="styles/default/menu.css" />
		<script type='text/javascript' src='scripts/main.js'></script>
		<script type='text/javascript' src='scripts/menu.js'></script>
		<script type='text/javascript' src='scripts/progress.js'></script>
		<script type='text/javascript' src='scripts/drag.js'></script>
        <title>NetFlow Statistics</title>
    </head>
    <body>
<div id="mainmenu" class="menu">
	<ul>
		<li><a id='page_link' href='' title='Current page link'><img src='img/page_link.png' alt='Current page link'></a></li>
		<li><span rel="ddm_sections">Sections</span></li>
		<li><span rel="ddm_view">View</span></li>
		<li><span rel="ddm_options">Options</span></li>
	</ul>
</div>
<div id="ddm_sections" class="dropdownmenu">
	<a href='index.php?section=traffic'>Networks</a>
</div>
<div id="ddm_view" class="dropdownmenu">
		<a href='index.php?view=protocols'><img src='img/t_protocol.png' alt='Protocol' />&nbsp;Protocols</a>
		<a href='index.php?view=hosts'><img src='img/t_host.png' alt='Host' />&nbsp;Hosts</a>
		<a href='index.php?view=ports'><img src='img/t_port.png' alt='Port' />&nbsp;Ports</a>
		<a href='index.php?view=tos'><img src='img/t_tos.png' alt='TOS' />&nbsp;Type of service</a>
		<a href='index.php?view=as'><img src='img/t_as.png' alt='AS' />&nbsp;Autonomous system</a>
</div>
<div id="ddm_options" class="dropdownmenu"><span></span></div>
<script type="text/javascript">tabdropdown.init('mainmenu',0);</script>

<br /><br />
_;
		$this->progress_header();

		$today = split(":", date('Y:m:d'));
		$hhl_day = '';
		$dates = new CDateTimeExtractor();

		$years	= "<table align='center'><tr>";
		foreach($dates->get_years() as $i => $year) 
		{
			if($today[0] == $year) { $hhl_day = 'today'; } else { $hhl_day = 'day'; }
			if($year == $this->current_date['year']) { $hhl_day .= ' viewday'; }
			$years .= "<th class='{$hhl_day}'><a href='index.php?dt={$year}'>{$year}</a></th>";
		}
		$years	.=	"</tr></table>";

		$months =	"<table align='center'><tr>";
		foreach($dates->get_months($this->current_date['year']) as $i => $month)
		{
			if($today[1] == $month) { $hhl_day = 'today'; } else { $hhl_day = 'day'; }
			if($month == $this->current_date['month']) { $hhl_day .= ' viewday'; }
			$months .= "<th class='{$hhl_day}'><a href='index.php?dt={$year}_{$month}'>{$month}</a></th>";
		}
		$months	.=	"</tr></table>";

		$days	=	"<table align='center'><tr>";
		if($this->current_date['month'] != '')
		{
		    foreach($dates->get_days($this->current_date['year'], $this->current_date['month']) as $i => $day)
		    {
				if($today[2] == $day) { $hhl_day = 'today'; } else { $hhl_day = 'day'; }
				if($day == $this->current_date['day']) { $hhl_day .= ' viewday'; }
				switch(date("w", mktime(0, 0, 0, $month, $day, $year)))
				{
					case 0:
					case 6: $hhl_day .= ' uickend'; break;
				}
				$days .= "<th class='{$hhl_day}'><a href='index.php?dt={$year}_{$month}_{$day}'>{$day}</a></th>";
		    }
			$days .= "</tr></table>";
		}
		else {$days = '';}
		$dates = null;
		echo <<<_
<table align='center' class='outer'>
<tr><th class='up' width='10%'>{$this->current_date['year']} {$this->current_date['month']} {$this->current_date['day']}</th><th class='up'><a href='index.php?session_clear=true'>PHP NetFlow GUI</a></th></tr>
<tr><td colspan='2'>{$years}{$months}{$days}</td></tr>
</table>
_;
		$this->set_progress_text('Headers done');
    }

	protected function show_window_table_header($name, $title, $append_to_menu = true)
	{
		$table_id = "{$name}_hu_table";
		if($append_to_menu)
		{
			echo "<script type='text/javascript'>append_to_menu('ddm_options', '{$title}', '{$table_id}');</script>";
        }
		echo <<<_
<table id='{$table_id}' class='twin_table' align='center'>
<tr class='unvisible'>
	<td class='unvisible'>
		<table align='center' class='unvisible'>
		<tr>
			<th class='up' id='{$table_id}_handle' style='cursor:move;'>{$title}<script type='text/javascript'>
//<![CDATA[
var exampB = new dragObject('{$table_id}', '{$table_id}_handle');
//]]>
</script></th>
			<th class='up' width='17px'><img src='img/w_close.png' alt='Close' title='Close' onclick='hu_close("{$name}_hu_table")' /></th>
		</tr>
		</table>
	</td>
</tr>
<tr class='unvisible'>
	<td class='unvisible'>
_;
		return $table_id;
    }
	
	protected function show_window_table_footer()
	{
		echo "</td></tr></table>";
    }

	abstract public function show();
	
	protected function convert_data_value($value)
	{
		global $sm;
		switch($sm->value('view'))
		{
			case 'hosts':	return ip2int($value); break;
			/*case 'protocols':break;
			case 'ports':break;*/
			default:		return $value;
        }
    }
	
	protected function get_header_data($headers, $default_column)
	{
		global $sm;
		$result = array();
		$vsf = $sm->value('section') . '_' . $sm->value('view') . '_sf';
		$vsd = $sm->value('section') . '_' . $sm->value('view') . '_sd';
		$sm->prepare_variable_batch(array($vsf, $vsd), TSM_STRING, array($default_column, 'asc'), true, TSM_GET);
		$result['table_header'] = "";
		$result['sort_field'] = $sm->value($vsf);
		$result['sort_order'] = $sm->value($vsd);
		foreach($headers as $name => $title)
		{
			$sort_order = 'asc';
			$img = '';
			if($name == $result['sort_field'])
			{
				$sort_order = $result['sort_order'] == 'asc' ? 'dsc' : 'asc';
				$img = "&nbsp;<img src='img/sort_{$sort_order}.png' alt='{$sort_order}'/>";
            }
			$result['table_header'] .= <<<_
<th>
	<a href='index.php?{$vsd}={$sort_order}&amp;{$vsf}={$name}'>
	{$title}{$img}
	</a>
</th>
_;
        }
		return $result;
    }

	protected function prepare_total($data)
	{
		$rows = count($data);
		$cells = "";
		foreach($data as $value) { $cells .= "<td>{$value}</td>"; }
		return <<<_
<tr><th colspan='{$rows}'>Total</th></tr>
<tr>{$cells}</tr>
_;
    }

    private function show_footer()
    {
		global $config;
		global $sm;
		$pagelink = $sm->get_link();
		$size = 0;
		$count = 0;
		$dh  = opendir($config['flow']['data_path']);
		while (false !== ($filename = readdir($dh)))
		{
			if($filename != "." && $filename != "..")
			{
				$size += filesize("{$config['flow']['data_path']}/{$filename}");
				$count++;
			}
        }
		closedir($dh);
		$size_per_file = num_format($size/$count, 'b');
		$size = num_format($size, 'b');
		$page_time = $this->page_generated_in();
        echo <<<_
<br />
<table align='center' class='outer'>
	<tr>
		<th>Copyright © 2008 by Sheridan</th>
		<th>flow-tools data size: {$size}. Files: {$count}, ~{$size_per_file} per file</th>
		<th>Page build time: {$page_time}</th>
	</tr>
</table>
_;
		$this->progress_footer();
		echo <<<_
<script type='text/javascript'>
//<![CDATA[
set_link("{$pagelink}");
//]]>
</script>
</body>
</html>
_;
    }

	private function getmicrotime()
	{
		list($usec, $sec) = explode(" ", microtime());
		return ((float)$usec + (float)$sec);
	}

	private function page_generated_in ()
	{
		global $st;
		return round($this->getmicrotime() - $this->page_time, 5);
	}
	
	
	private function progress_header()
	{
		global $config;
		if(!$config['progress']['show']) { return; }
		echo <<<_
<center>
<div id='progress' class='progress'>
<table align='center'>
<tr>
	<td style='background-color: transparent; border-style: none;' width='3%'><img src='img/progress.gif' title='Progess' alt='Progress' /></td>
	<td style='background-color: transparent; border-style: none;'><div id='progress_text' class='progress_text'>Hello</div></td>
</tr>
</table>
</div>
</center>
_;
    }
	
	private function progress_footer()
	{
		global $config;
		if(!$config['progress']['show']) { return; }
		echo <<<_
<script type='text/javascript'>
//<![CDATA[
hide();
//]]>
</script>
_;
    }
	
	public function set_progress_text($text)
	{
		global $config;
		if(!$config['progress']['show']) { return; }
		echo <<<_
<script type='text/javascript'>
//<![CDATA[
progress_show_text('{$text}');
//]]>
</script>
_;
		flush();
		ob_flush();
    }

	public function progressbar_init($title, $maximum)
	{
		global $config;
		if(!$config['progress']['show']) { return; }
		$this->progress['title'] = $title;
		$this->progress['one'] = $maximum/100;
		$this->progress['max'] = $maximum;
		$this->progress['delta'] = 0;
		$this->progress['current'] = 0;
		$this->set_progress_text("Initialise {$title}");
    }
	
	public function progressbar_step($current = 1)
	{
		global $config;
		if(!$config['progress']['show']) { return; }
		$this->progress['current']+=$current;
		$work_done = $this->progress['one'] ? round($this->progress['current']/$this->progress['one']) : 0;
		if($work_done - $this->progress['delta'] >= $config['progress']['show_every'])
		{
			if($work_done == 100) { $this->progress['current'] = $this->progress['max']; }
			echo <<<_
<script type='text/javascript'>
//<![CDATA[
progress_show_gauge('{$this->progress['title']}', {$work_done}, {$this->progress['max']}, {$this->progress['current']});
//]]>
</script>
_;
			flush();
			ob_flush();
			$this->progress['delta'] = $work_done;
		}
    }
}
?>
