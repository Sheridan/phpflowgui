<?php

function num_format($num, $postfix = null)
{
	$num = number_format($num, 0, '.', "'");
    if($postfix) { $num = "<b>{$num}</b>{$postfix}"; }
    return $num;
}

function int2ip(&$i)
{
	$d = array();
	$d[0]=(int)($i/256/256/256);
	$d[1]=(int)(($i-$d[0]*256*256*256)/256/256);
	$d[2]=(int)(($i-$d[0]*256*256*256-$d[1]*256*256)/256);
	$d[3]=$i-$d[0]*256*256*256-$d[1]*256*256-$d[2]*256;
	return "$d[0].$d[1].$d[2].$d[3]";
}

function ip2int(&$ip)
{
	$a=explode(".",$ip);
	return $a[0]*256*256*256+$a[1]*256*256+$a[2]*256+$a[3];
}

function int2subnet(&$i)
{
	$s = array();
	$s[0] = (int) ($i/256/256/256/256);
	$s[1] = (int)(($i-$s[0]*256*256*256*256)/256/256/256);
	$s[2] = (int)(($i-$s[0]*256*256*256*256-$s[1]*256*256*256)/256/256);
	$s[3] = (int)(($i-$s[0]*256*256*256*256-$s[1]*256*256*256-$s[2]*256*256)/256);
	$s[4] = (int) ($i-$s[0]*256*256*256*256-$s[1]*256*256*256-$s[2]*256*256-$s[3]*256);
	return "$s[0].$s[1].$s[2].$s[3]/$s[4]";
}

function subnet2int(&$s)
{
	$is = explode("/", $s);
	$a = explode(".", $is[0]);
	return $a[0]*256*256*256*256+$a[1]*256*256*256+$a[2]*256*256+$a[3]*256+$is[1];
}

function sdatea2sdate(&$sdarr) { return date2sdate($sdarr['year'],$sdarr['month'],$sdarr['day']); }
function date2sdate(&$year, &$month = '', &$day = '') { return $year . ($month != '' ? "_{$month}" . ($day != '' ? "_{$day}" : '') : ''); }
function sdate2date(&$sdate)
{
	$sd=explode("_",$sdate);
	return array('year' => $sd[0], 'month' => isset($sd[1]) ? $sd[1] : '', 'day' => isset($sd[2]) ? $sd[2] : '');
}

function is_ip(&$ip) { return preg_match('/^([0-9]{1,3}\.){3}[0-9]{1,3}$/', $ip) > 0; }
function is_network(&$net) { return preg_match('/^([0-9]{1,3}\.){3}[0-9]{1,3}\/[0-9]{1,2}$/', $net) > 0; }
function isint(&$i) { return preg_match('/^[0-9]*$/', $i) > 0; }

function max_int(&$a, &$b) { return $a >= $b ? $a : $b; }
function min_int(&$a, &$b) { return $a <= $b ? $a : $b; }
function max_ip (&$a, &$b) { return int2ip(max_int(ip2int($a), ip2int($b))); }
function min_ip (&$a, &$b) { return int2ip(min_int(ip2int($a), ip2int($b))); }
function max_str(&$a, &$b) { return strcasecmp($a, $b) >=0 ? $a : $b; }
function min_str(&$a, &$b) { return strcasecmp($a, $b) <=0 ? $a : $b; }

function cmp_int(&$a, &$b) { if ($a == $b) { return 0; } return ($a > $b) ? -1 : 1; }
function cmp_ip (&$a, &$b) { return cmp_int(ip2int($a), ip2int($b)); }
function cmp_str(&$a, &$b) { return strcasecmp($b, $a); }
function rcmp_int(&$a, &$b) { return cmp_int($b, $a); }
function rcmp_ip (&$a, &$b) { return cmp_ip($b, $a); }
function rcmp_str(&$a, &$b) { return strcasecmp($a, $b); }

function rcmp_strint(&$a, &$b)
{
	$a_is_int = isint($a);
	$b_is_int = isint($b);
	if ($a_is_int && $b_is_int) { return cmp_int($a, $b); }
	else
	{
		if ($a_is_int) return -1;
		if ($b_is_int) return 1;
		return cmp_str($a, $b);
    }
}
function cmp_strint(&$a, &$b) { return rcmp_strint($b, $a); }

function rcmp_hostname(&$a, &$b)
{
	$a_is_ip = is_ip($a);
	$b_is_ip = is_ip($b);
	if ($a_is_ip && $b_is_ip) { return cmp_ip($a, $b); }
	else
	{
		if ($a_is_ip) return -1;
		if ($b_is_ip) return 1;
		return cmp_str(implode('.', array_reverse(explode(".", $a), true)), implode('.', array_reverse(explode(".", $b), true)));
    }
}

function cmp_hostname(&$a, &$b) { return rcmp_hostname($b, $a); }

function dprint($caption, &$val) { echo "<table><tr><th>{$caption}</th></tr><tr><td><pre>"; print_r($val); echo '</pre></td></tr></table>'; }
function dvdump($caption, &$val) { echo "<table><tr><th>{$caption}</th></tr><tr><td><pre>"; var_dump($val); echo '</pre></td></tr></table>'; }
function dpgprint($text) { global $page; $page->set_progress_text($text); }

?>
