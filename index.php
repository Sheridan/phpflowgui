<?php

//error_reporting(E_ALL);
ini_set("memory_limit","128M");
ini_set("max_execution_time",600);
include_once('config.dist.php');
include_once('src/functions.inc');
include_once('src/sessionmanager.inc');

global $config;
/*foreach($config['networks'] as $dir => $network)
{
	list($id, $ar) = $network;
	dprint($dir, $ar['network']);
}*/
/*$ext_exists = false;
foreach($config['filter']['network'] as $network)
{
	if($network['data'] == '0.0.0.0/0')
	{
		$ext_exists = true;
		break;
    }
}
if (!$ext_exists) { $config['filter']['network'][] = array('name' => 'External networks', 'data' => '0.0.0.0/0'); }*/
$config['filter']['host'][] = array('name' => 'This host', 'data' => $_SERVER['REMOTE_ADDR']); 

define('TT_DATA_CAPTION', 0);
define('TT_LABEL_CAPTION', 1);
define('TT_DATA_TYPE', 2);
//'network' 'host' 'port' 'protocol' 'tos' 'as'
$data_types = array
		(
			'network'	=> array(TT_DATA_CAPTION => 'Network'			, TT_LABEL_CAPTION => 'Network name'		, TT_DATA_TYPE => TSM_SUBNET),
			'host'		=> array(TT_DATA_CAPTION => 'IP address'		, TT_LABEL_CAPTION => 'Host name'			, TT_DATA_TYPE => TSM_IP),
			'port'		=> array(TT_DATA_CAPTION => 'Port'				, TT_LABEL_CAPTION => 'Service'				, TT_DATA_TYPE => TSM_INT),
			'protocol'	=> array(TT_DATA_CAPTION => 'Protocol'			, TT_LABEL_CAPTION => 'Protocol name'		, TT_DATA_TYPE => TSM_INT),
			'tos'		=> array(TT_DATA_CAPTION => 'Type of service'	, TT_LABEL_CAPTION => 'Type of service'		, TT_DATA_TYPE => TSM_INT),
			'as'		=> array(TT_DATA_CAPTION => 'Autonomous system'	, TT_LABEL_CAPTION => 'Autonomous system'	, TT_DATA_TYPE => TSM_INT)
		);

/*
global $config;
dprint($config);
echo int2subnet(subnet2int("252.231.247.211/32"));
 */

$sm = new SessionManager();		//dprint('Session', $_SESSION);dprint('Get', $_GET);dprint('Post', $_POST);
$page = null;
$sm->prepare_variable('section', TSM_STRING, 'traffic', true, TSM_GET);
switch($sm->value('section'))
{
	case 'traffic':
	{
		include_once('src/pages/trafficpage.inc');
		$page = new TrafficPage();
    }
	break;
}
$page->show();
$page = null;
$sm = null;

?>
