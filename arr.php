<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html>
    <head>
        <meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
        <title>test</title>
	<style>
		div.graph
		{
			margin: 0px;
			padding: 0px;
			text-indent: 0px;
			word-spacing: 0px;
			letter-spacing: 0px;
			line-height: 1px;
			font-size: 2px;
		}
	</style>
    </head>
    <body>
<?php

function draw($percents, $is_seconds = false)
{
	$max_x = 100;
	$zoom_x = 2;
	$cpt = '%';
	$lines = count($percents);
	if($is_seconds)
	{
		$cpt = 'Seconds';
		$max = 0;
		for($pi = 0; $pi < $lines; $pi++) { if($percents[$pi][1] > $max) { $max = $percents[$pi][1]; } }
		$max_x = $max;
		$zoom_x = 100/$max;
    }
	$graph_line_thing = 10;
	$graph_line_spacing = 2;
	for($pi = 0; $pi < $lines; $pi++) { $percents[$pi]['lt'] = 0;  $percents[$pi]['ls'] = 0; $percents[$pi]['x'] = $percents[$pi][1]*$zoom_x; }
	$percents[$lines-1]['ls'] = $graph_line_spacing;
	//print_r($percents);
	$max_y = ($graph_line_thing+$graph_line_spacing)*$lines;
	$max_x = $zoom_x * $max_x;
	$char = '8';
	$color = 'black';
	$curr_color = '';
	echo "<div class='graph'>";
	echo "<font color='black'>"; for($y = 0; $y < $graph_line_spacing*2; $y++) { for($x = 0; $x < $max_x; $x++) { echo $char; } echo "<br />"; } echo "</font>";
	$index = 0;
	echo "<font color='{$color}'>";
    for($y = 0; $y < $max_y; $y++)
    {
		for($pi = 0; $pi < $lines; $pi++)
		{
			if($percents[$pi]['lt'] <= $graph_line_thing)
			{
				$index = $pi;
				$percents[$pi]['lt']++;
				break;
			}
			//if($pi == $lines-1 && $percents[$pi]['lt'] >= $graph_line_thing) { $percents[$pi]['ls'] = $graph_line_spacing; }
			if($percents[$pi]['ls'] <= $graph_line_spacing)
			{
				$index = $pi;
				$percents[$pi]['ls']++;
				break;
            }
        }
		for($x = 0; $x < $max_x; $x++)
		{
			if($x && $x%20 == 0) { $color = 'darkgray'; } else { $color = 'black'; }
			if($percents[$index]['lt'] < $graph_line_thing && $percents[$index]['x'] >= $x) { $color = $percents[$index][0]; }
			if($color != $curr_color) { echo "</font><font color='{$color}'>{$char}"; $curr_color = $color; } else { echo $char; }
		}
		echo "<br />";
    }
	echo "</font>";
	echo "<font color='black'>"; for($y = 0; $y < $graph_line_spacing*2; $y++) { for($x = 0; $x < $max_x; $x++) { echo $char; } echo "<br />"; } echo "</font>";
	echo "</div>";
	echo "<ul>";
	for($pi = 0; $pi < $lines; $pi++)
	{
		echo "<li style='color: {$percents[$pi][0]};'><font color='black'>{$percents[$pi][1]} {$cpt}: {$percents[$pi][2]}</font></li>";
    }
	echo "</ul>";
	//print_r($percents);
}

function i($cmd)
{
    $out = null;
    exec($cmd, &$out);
    echo "<pre>~# {$cmd}\n";
    foreach($out as $line)
    {
	echo $line . "\n";
    }
    echo "</pre>";
}

ini_set("memory_limit", "256M");
ini_set("max_execution_time", 600);

$round = 20;
$tmp = null;

function pc($name_1, $name_2, $time_1, $time_2, $color)
{
    if(($time_1 - $time_2) > 0)
    {
		$perc = round((100 * ($time_1 - $time_2))/$time_1, 2);
		echo "'{$name_2}' faster than the '{$name_1}' to {$perc}%<br />";
		draw(array(array('red', 100, $name_2), array('green', 100-$perc, $name_1)));
		return array($color, $perc, "'{$name_2}' > '{$name_1}'");
    }
	return null;
}

function d_1($ipl)
{
    $f_time_start = microtime(1);
    echo "<b>1 dimension</b><br />";
    global $round;
    $total = 0;
    $arr = array();
    $time_start_gen = microtime(1);
    for($i = 0; $i < $ipl; $i++)
    {
        $arr[$i] = 1;
	$total++;
    }
    $time_end_gen = microtime(1);
    $time_gen = round($time_end_gen - $time_start_gen, $round);
    echo "Generating array (for): {$time_gen} s. 1 dimension of ${ipl} items. Total items: {$total}<br />";
    flush();
    ob_flush();
    //------------------------------------------------------------
    $total = 0;
    $time_start_for = microtime(1);
    $ni = sizeof($arr);
    for($i = 0; $i < $ni; $i++)
    {
	$a = $arr[$i];
	$total++;
    }
    $time_end_for = microtime(1);
    $time_for = round($time_end_for - $time_start_for, $round);
    echo "Result 'for': {$time_for}s, {$total} items<br />";
    flush();
    ob_flush();
    //------------------------------------------------------------
    $total = 0;
    $i = 0;
    $time_start_while = microtime(1);
    $ni = sizeof($arr);
    while($i < $ni)
    {
        $a = $arr[$i];
	$total++;
	$i++;
    }
    $time_end_while = microtime(1);
    $time_while = round($time_end_while - $time_start_while, $round);
    echo "Result 'while': {$time_while}s, {$total} items<br />";
    flush();
    ob_flush();
    //------------------------------------------------------------
    $total = 0;
    $time_start_foreach = microtime(1);
    foreach($arr as $key => $val)
    {
	$a = $val;
	$total++;
    }
    $time_end_foreach = microtime(1);
    $time_foreach = round($time_end_foreach - $time_start_foreach, $round);
    echo "Result 'foreach': {$time_foreach}s, {$total} items<br />";
    //------------------------------------------------------------
    echo '<br />';
	$chart = array();
    $tmp = pc('for', 'while', $time_for, $time_while, 'red'); if($tmp) { $chart[] = $tmp; $tmp = null; }
    $tmp = pc('for', 'foreach', $time_for, $time_foreach, 'blue'); if($tmp) { $chart[] = $tmp; $tmp = null; }
    $tmp = pc('while', 'foreach', $time_while, $time_foreach, 'green'); if($tmp) { $chart[] = $tmp; $tmp = null; }
    $tmp = pc('while', 'for', $time_while, $time_for, 'orange'); if($tmp) { $chart[] = $tmp; $tmp = null; }
    $tmp = pc('foreach', 'for', $time_foreach, $time_for, 'pink'); if($tmp) { $chart[] = $tmp; $tmp = null; }
    $tmp = pc('foreach', 'while', $time_foreach, $time_while, 'brown'); if($tmp) { $chart[] = $tmp; $tmp = null; }
	draw($chart);

    $f_time_end = microtime(1);
    $f_time = round($f_time_end - $f_time_start, $round);
    echo "<b>1d result: {$f_time}s</b><br />";
    echo '<hr />';
    $arr = null;
    flush();
    ob_flush();
    return $f_time;
}

function d_2($ipl)
{
    $f_time_start = microtime(1);
    echo "<b>2 dimensions</b><br />";
    global $round;
    $total = 0;
    $arr = array();
    $time_start_gen = microtime(1);
    for($i = 0; $i < $ipl; $i++)
    {
        for($j = 0; $j < $ipl; $j++)
        {
	    $arr[$i][$j] = 1;
	    $total++;
	}
    }
    $time_end_gen = microtime(1);
    $time_gen = round($time_end_gen - $time_start_gen, $round);
    echo "Generating array (for): {$time_gen} s. 2 dimensions of ${ipl} items. Total items: {$total}<br />";
    flush();
    ob_flush();
    //------------------------------------------------------------
    $total = 0;
    $time_start_for = microtime(1);
    $ni = sizeof($arr);
    for($i = 0; $i < $ni; $i++)
    {
	$nj = sizeof($arr[$i]);
        for($j = 0; $j < $nj; $j++)
	{
	    $a = $arr[$i][$j];
	    $total++;
	}
    }
    $time_end_for = microtime(1);
    $time_for = round($time_end_for - $time_start_for, $round);
    echo "Result 'for': {$time_for}s, {$total} items<br />";
    flush();
    ob_flush();
    //------------------------------------------------------------
    $total = 0;
    $time_start_while = microtime(1);
    $ni = sizeof($arr);
    $i = 0;
    while($i < $ni)
    {
	$j = 0;
	$nj = sizeof($arr[$i]);
        while($j < $nj)
	{
    	    $a = $arr[$i][$j];
	    $total++;
	    $j++;
	}
	$i++;
    }
    $time_end_while = microtime(1);
    $time_while = round($time_end_while - $time_start_while, $round);
    echo "Result 'while': {$time_while}s, {$total} items<br />";
    flush();
    ob_flush();
    //------------------------------------------------------------
    $total = 0;
    $time_start_foreach = microtime(1);
    foreach($arr as $keyi => $datai)
    {
	foreach($datai as $keyj => $val)
	{
	    $a = $val;
	    $total++;
	}
    }
    $time_end_foreach = microtime(1);
    $time_foreach = round($time_end_foreach - $time_start_foreach, $round);
    echo "Result 'foreach': {$time_foreach}s, {$total} items<br />";
    //------------------------------------------------------------
    echo '<br />';
    $chart = array();
    $tmp = pc('for', 'while', $time_for, $time_while, 'red'); if($tmp) { $chart[] = $tmp; $tmp = null; }
    $tmp = pc('for', 'foreach', $time_for, $time_foreach, 'blue'); if($tmp) { $chart[] = $tmp; $tmp = null; }
    $tmp = pc('while', 'foreach', $time_while, $time_foreach, 'green'); if($tmp) { $chart[] = $tmp; $tmp = null; }
    $tmp = pc('while', 'for', $time_while, $time_for, 'orange'); if($tmp) { $chart[] = $tmp; $tmp = null; }
    $tmp = pc('foreach', 'for', $time_foreach, $time_for, 'pink'); if($tmp) { $chart[] = $tmp; $tmp = null; }
    $tmp = pc('foreach', 'while', $time_foreach, $time_while, 'brown'); if($tmp) { $chart[] = $tmp; $tmp = null; }
	draw($chart);
    $f_time_end = microtime(1);
    $f_time = round($f_time_end - $f_time_start, $round);
    echo "<b>2d result: {$f_time}s</b><br />";
    echo '<hr />';
    $arr = null;
    flush();
    ob_flush();
    return $f_time;
}

function d_3($ipl)
{
    $f_time_start = microtime(1);
    echo "<b>3 dimensions</b><br />";
    global $round;
    $total = 0;
    $arr = array();
    $time_start_gen = microtime(1);
    for($i = 0; $i < $ipl; $i++)
    {
        for($j = 0; $j < $ipl; $j++)
        {
    	    for($k = 0; $k < $ipl; $k++)
    	    {
		$arr[$i][$j][$k] = 1;
		$total++;
	    }
	}
    }
    $time_end_gen = microtime(1);
    $time_gen = round($time_end_gen - $time_start_gen, $round);
    echo "Generating array (for): {$time_gen} s. 3 dimensions of ${ipl} items. Total items: {$total}<br />";
    flush();
    ob_flush();
    //------------------------------------------------------------
    $total = 0;
    $time_start_for = microtime(1);
    $ni = sizeof($arr);
    for($i = 0; $i < $ni; $i++)
    {
	$nj = sizeof($arr[$i]);
        for($j = 0; $j < $nj; $j++)
	{
	    $nk = sizeof($arr[$i][$j]);
	    for($k = 0; $k < $nk; $k++)
	    {
		$a = $arr[$i][$j][$k];
		$total++;
	    }
	}
    }
    $time_end_for = microtime(1);
    $time_for = round($time_end_for - $time_start_for, $round);
    echo "Result 'for': {$time_for}s, {$total} items<br />";
    flush();
    ob_flush();
    //------------------------------------------------------------
    $total = 0;
    $time_start_while = microtime(1);
    $ni = sizeof($arr);
    $i = 0;
    while($i < $ni)
    {
	$j = 0;
	$nj = sizeof($arr[$i]);
        while($j < $nj)
	{
	    $k = 0;
	    $nk = sizeof($arr[$i][$j]);
	    while($k < $nk)
	    {
    		$a = $arr[$i][$j][$k];
		$total++;
		$k++;
	    }
	    $j++;
	}
	$i++;
    }
    $time_end_while = microtime(1);
    $time_while = round($time_end_while - $time_start_while, $round);
    echo "Result 'while': {$time_while}s, {$total} items<br />";
    flush();
    ob_flush();
    //------------------------------------------------------------
    $total = 0;
    $time_start_foreach = microtime(1);
    foreach($arr as $keyi => $datai)
    {
	foreach($datai as $keyj => $dataj)
	{
	    foreach($dataj as $keyk => $val)
	    {
		$a = $val;
		$total++;
	    }
	}
    }
    $time_end_foreach = microtime(1);
    $time_foreach = round($time_end_foreach - $time_start_foreach, $round);
    echo "Result 'foreach': {$time_foreach}s, {$total} items<br />";
    //------------------------------------------------------------
    echo '<br />';
    $chart = array();
    $tmp = pc('for', 'while', $time_for, $time_while, 'red'); if($tmp) { $chart[] = $tmp; $tmp = null; }
    $tmp = pc('for', 'foreach', $time_for, $time_foreach, 'blue'); if($tmp) { $chart[] = $tmp; $tmp = null; }
    $tmp = pc('while', 'foreach', $time_while, $time_foreach, 'green'); if($tmp) { $chart[] = $tmp; $tmp = null; }
    $tmp = pc('while', 'for', $time_while, $time_for, 'orange'); if($tmp) { $chart[] = $tmp; $tmp = null; }
    $tmp = pc('foreach', 'for', $time_foreach, $time_for, 'pink'); if($tmp) { $chart[] = $tmp; $tmp = null; }
    $tmp = pc('foreach', 'while', $time_foreach, $time_while, 'brown'); if($tmp) { $chart[] = $tmp; $tmp = null; }
	draw($chart);
    $f_time_end = microtime(1);
    $f_time = round($f_time_end - $f_time_start, $round);
    echo "<b>3d result: {$f_time}s</b><br />";
    echo '<hr />';
    $arr = null;
    flush();
    ob_flush();
    return $f_time;
}

function d_6($ipl)
{
    $f_time_start = microtime(1);
    echo "<b>6 dimensions</b><br />";
    global $round;
    $total = 0;
    $arr = array();
    $time_start_gen = microtime(1);
    for($i = 0; $i < $ipl; $i++)
    {
        for($j = 0; $j < $ipl; $j++)
        {
    	    for($k = 0; $k < $ipl; $k++)
    	    {
		for($l = 0; $l < $ipl; $l++)
    		{
		    for($o = 0; $o < $ipl; $o++)
    		    {
			for($z = 0; $z < $ipl; $z++)
    			{
			    $arr[$i][$j][$k][$l][$o][$z] = 1;
		    	    $total++;
			}
		    }
		}
	    }
	}
    }
    $time_end_gen = microtime(1);
    $time_gen = round($time_end_gen - $time_start_gen, $round);
    echo "Generating array (for): {$time_gen} s. 4 dimensions of ${ipl} items. Total items: {$total}<br />";
    flush();
    ob_flush();
    //------------------------------------------------------------
    $total = 0;
    $time_start_for = microtime(1);
    $ni = sizeof($arr);
    for($i = 0; $i < $ni; $i++)
    {
	$nj = sizeof($arr[$i]);
        for($j = 0; $j < $nj; $j++)
	{
	    $nk = sizeof($arr[$i][$j]);
	    for($k = 0; $k < $nk; $k++)
	    {
		$nl = sizeof($arr[$i][$j][$k]);
		for($l = 0; $l < $nl; $l++)
		{
		    $no = sizeof($arr[$i][$j][$k][$l]);
		    for($o = 0; $o < $no; $o++)
		    {
			$nz = sizeof($arr[$i][$j][$k][$l][$o]);
			for($z = 0; $z < $nz; $z++)
			{
			    $a = $arr[$i][$j][$k][$l][$o][$z];
			    $total++;
			}
		    }
		}
	    }
	}
    }
    $time_end_for = microtime(1);
    $time_for = round($time_end_for - $time_start_for, $round);
    echo "Result 'for': {$time_for}s, {$total} items<br />";
    flush();
    ob_flush();
    //------------------------------------------------------------
    $total = 0;
    
    $time_start_while = microtime(1);
    $ni = sizeof($arr);
    $i = 0;
    while($i < $ni)
    {
	$j = 0;
	$nj = sizeof($arr[$i]);
        while($j < $nj)
	{
	    $k = 0;
	    $nk = sizeof($arr[$i][$j]);
	    while($k < $nk)
	    {
		$l = 0;
		$nl = sizeof($arr[$i][$j][$k]);
		while($l < $nl)
		{
		    $o = 0;
		    $no = sizeof($arr[$i][$j][$k][$l]);
		    while($o < $no)
		    {
			$z = 0;
			$nz = sizeof($arr[$i][$j][$k][$l][$o]);
			while($z < $nz)
			{
    			    $a = $arr[$i][$j][$k][$l][$o][$z];
			    $total++;
			    $z++;
			}
			$o++;
		    }
		    $l++;
		}
		$k++;
	    }
	    $j++;
	}
	$i++;
    }
    $time_end_while = microtime(1);
    $time_while = round($time_end_while - $time_start_while, $round);
    echo "Result 'while': {$time_while}s, {$total} items<br />";
    flush();
    ob_flush();
    //------------------------------------------------------------
    $total = 0;
    $time_start_foreach = microtime(1);
    foreach($arr as $keyi => $datai)
    {
	foreach($datai as $keyj => $dataj)
	{
	    foreach($dataj as $keyk => $datak)
	    {
		foreach($datak as $keyl => $datal)
		{
		    foreach($datal as $keyo => $datao)
		    {
			foreach($datao as $keyz => $val)
			{
		    	    $a = $val;
			    $total++;
			}
		    }
		}
	    }
	}
    }
    $time_end_foreach = microtime(1);
    $time_foreach = round($time_end_foreach - $time_start_foreach, $round);
    echo "Result 'foreach': {$time_foreach}s, {$total} items<br />";
    //------------------------------------------------------------
    echo '<br />';
    $chart = array();
    $tmp = pc('for', 'while', $time_for, $time_while, 'red'); if($tmp) { $chart[] = $tmp; $tmp = null; }
    $tmp = pc('for', 'foreach', $time_for, $time_foreach, 'blue'); if($tmp) { $chart[] = $tmp; $tmp = null; }
    $tmp = pc('while', 'foreach', $time_while, $time_foreach, 'green'); if($tmp) { $chart[] = $tmp; $tmp = null; }
    $tmp = pc('while', 'for', $time_while, $time_for, 'orange'); if($tmp) { $chart[] = $tmp; $tmp = null; }
    $tmp = pc('foreach', 'for', $time_foreach, $time_for, 'pink'); if($tmp) { $chart[] = $tmp; $tmp = null; }
    $tmp = pc('foreach', 'while', $time_foreach, $time_while, 'brown'); if($tmp) { $chart[] = $tmp; $tmp = null; }
	draw($chart);
    $f_time_end = microtime(1);
    $f_time = round($f_time_end - $f_time_start, $round);
    echo "<b>6d result: {$f_time}s</b><br />";
    echo '<hr />';
    $arr = null;
    flush();
    ob_flush();
    return $f_time;
}

function run()
{
	echo "<h1>PC info</h1>";
	i('uname -a');
	i('cat /proc/cpuinfo | grep "model name"');
	i('free');
	i('uptime');
	i('php -v');
	i('apache2 -v');

    echo "<h1>Equal total items</h1>";
    $items_per_level = 10;
    $d_1t = d_1(pow($items_per_level, 6));
    $d_2t = d_2(pow($items_per_level, 3));
    $d_3t = d_3(pow($items_per_level, 2));
    $d_6t = d_6(pow($items_per_level, 1));
    
	$chart = array();
    $tmp = pc('6d', '1d', $d_6t, $d_1t, 'red'); if($tmp) { $chart[] = $tmp; $tmp = null; }
    $tmp = pc('6d', '2d', $d_6t, $d_2t, 'blue'); if($tmp) { $chart[] = $tmp; $tmp = null; }
    $tmp = pc('6d', '3d', $d_6t, $d_3t, 'green'); if($tmp) { $chart[] = $tmp; $tmp = null; }
    $tmp = pc('3d', '1d', $d_3t, $d_1t, 'brown'); if($tmp) { $chart[] = $tmp; $tmp = null; }
    $tmp = pc('3d', '2d', $d_3t, $d_2t, 'orange'); if($tmp) { $chart[] = $tmp; $tmp = null; }
    $tmp = pc('3d', '6d', $d_3t, $d_6t, 'darkred'); if($tmp) { $chart[] = $tmp; $tmp = null; }
    $tmp = pc('2d', '1d', $d_2t, $d_1t, 'navy'); if($tmp) { $chart[] = $tmp; $tmp = null; }
    $tmp = pc('2d', '3d', $d_2t, $d_3t, 'silver'); if($tmp) { $chart[] = $tmp; $tmp = null; }
    $tmp = pc('2d', '6d', $d_2t, $d_6t, 'darkgold'); if($tmp) { $chart[] = $tmp; $tmp = null; }
    $tmp = pc('1d', '2d', $d_1t, $d_2t, 'darkblue'); if($tmp) { $chart[] = $tmp; $tmp = null; }
    $tmp = pc('1d', '3d', $d_1t, $d_3t, 'lightgreen'); if($tmp) { $chart[] = $tmp; $tmp = null; }
    $tmp = pc('1d', '6d', $d_1t, $d_6t, 'mellow'); if($tmp) { $chart[] = $tmp; $tmp = null; }
	draw($chart);
	
    echo "<h1>Equal count items per level</h1>";
    $items_per_level = 12;
    $d_1t = d_1($items_per_level);
    $d_2t = d_2($items_per_level);
    $d_3t = d_3($items_per_level);
    $d_6t = d_6($items_per_level);
    
    $chart = array();
    $tmp = pc('6d', '1d', $d_6t, $d_1t, 'red'); if($tmp) { $chart[] = $tmp; $tmp = null; }
    $tmp = pc('6d', '2d', $d_6t, $d_2t, 'blue'); if($tmp) { $chart[] = $tmp; $tmp = null; }
    $tmp = pc('6d', '3d', $d_6t, $d_3t, 'green'); if($tmp) { $chart[] = $tmp; $tmp = null; }
    $tmp = pc('3d', '1d', $d_3t, $d_1t, 'brown'); if($tmp) { $chart[] = $tmp; $tmp = null; }
    $tmp = pc('3d', '2d', $d_3t, $d_2t, 'orange'); if($tmp) { $chart[] = $tmp; $tmp = null; }
    $tmp = pc('3d', '6d', $d_3t, $d_6t, 'darkred'); if($tmp) { $chart[] = $tmp; $tmp = null; }
    $tmp = pc('2d', '1d', $d_2t, $d_1t, 'navy'); if($tmp) { $chart[] = $tmp; $tmp = null; }
    $tmp = pc('2d', '3d', $d_2t, $d_3t, 'silver'); if($tmp) { $chart[] = $tmp; $tmp = null; }
    $tmp = pc('2d', '6d', $d_2t, $d_6t, 'darkgold'); if($tmp) { $chart[] = $tmp; $tmp = null; }
    $tmp = pc('1d', '2d', $d_1t, $d_2t, 'darkblue'); if($tmp) { $chart[] = $tmp; $tmp = null; }
    $tmp = pc('1d', '3d', $d_1t, $d_3t, 'lightgreen'); if($tmp) { $chart[] = $tmp; $tmp = null; }
    $tmp = pc('1d', '6d', $d_1t, $d_6t, 'mellow'); if($tmp) { $chart[] = $tmp; $tmp = null; }
	draw($chart);
}

run();
//draw(array(array('red', 60, 'first'), array('blue', 20, 'second'), array('green', 87, 'third')));

?>
</body>
</html>