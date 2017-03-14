<?php
$config['flow'] = array 
	(
		'filename_prefix'	=> 'ft-v05.',
		'data_path'			=> '/var/lib/flows/ft',
		'descriptions_path'	=> '/etc/flow-tools/sym'
	);
$config['temporally_directory'] = '/tmp';

// filter defaults
$config['filter']['network'] = array
	(
		array('name' => 'Local network', 'data' => '192.168.1.0/24'),
		array('name' => 'Mineralovodskiy RUS', 'data' => '88.215.128.0/18')
	);
$config['filter']['host'] = array(array('name' => 'Gate external', 'data' => '88.215.138.24'));
$config['filter']['port'] = array
	(
		array('name' => 'http', 'data' => 80),
		array('name' => 'irc', 'data' => 6667),
		array('name' => 'svn', 'data' => 3190),
		array('name' => 'ftp', 'data' => 21),
		array('name' => 'http proxy', 'data' => 3128),
		array('name' => 'domain', 'data' => 53),
		array('name' => 'ssh', 'data' => 22)
	);
$config['filter']['protocol'] = array(array('name' => 'tcp', 'data' => 6));
$config['filter']['tos'] = array();
$config['filter']['as'] = array();
$config['external_ip_addresses'] = array
	(
		array('name' => 'Internet', 'ip' => '88.215.138.24')
    );

//values in hours
$config['cache']['dns'] = array('check_every' => 48, 'remove_after' => 672);

//progressbar
$config['progress'] = array
	(
		'show'			=> true,
		'show_every'	=> 1
	);

if(file_exists("config.php")) { include_once("config.php"); }
?>
