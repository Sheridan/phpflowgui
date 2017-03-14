<?php
//Откуда берем данные и в каком порядке
define('TSM_POST_GET',	0);
define('TSM_GET_POST',	1);
define('TSM_POST',		2);
define('TSM_GET',		3);

// Типы данных
define('TSM_INT',		0);
define('TSM_STRING',	1);
define('TSM_BOOL',		2);
define('TSM_IP',		3);
define('TSM_SUBNET',	4);
define('TSM_SDATE',		5);

//Данные
define('TSM_DATA_TYPE',	0);
define('TSM_DATA',		1);
define('TSM_IS_ARRAY',	2);

//Переменная - значение
define('TSM_V_VALUE',	0);
define('TSM_V_INCOMED',	1);

// Вспмогательное
define('TSM_TRUE_REGEXP', '^(on|yes|y|t|true)$');
define('TSM_DELIMITER', ',');

class SessionManager
{
	private $storage;

    function __construct()
    {
		session_start();
		$this->prepare_variable('session_clear', TSM_BOOL, false, false, TSM_GET);
		if($this->value('session_clear'))
		{
			$_SESSION = array();
        }
    }

    function __destruct()
    {
		//dprint('sm', $this->storage);
    }
	
	private function set_session($name, $value)
	{
		if($value[TSM_V_VALUE]) { $_SESSION[$name] = $value[TSM_V_VALUE]; }
		else { unset($_SESSION[$name]); }
    }

	private function set_session_value($name, $value)
	{
		$this->set_session($name, $value);
	}
	
	private function get_ass_def_value(&$ass_array, $value_name, $default, $is_session = false)
	{
		if(array_key_exists($value_name, $ass_array))
		{
			$value = $ass_array[$value_name] != '' ? $ass_array[$value_name] : null;
			if(!$is_session) { unset($ass_array[$value_name]); }
			return array(TSM_V_INCOMED => !$is_session, TSM_V_VALUE => $value);
        }
		return $default;
	}
	
	private function get_session	($name, $default) { return $this->get_ass_def_value(&$_SESSION,	$name, $default, true);	}
	private function get_get		($name, $default) { return $this->get_ass_def_value(&$_GET,		$name, $default);	}
	private function get_post		($name, $default) { return $this->get_ass_def_value(&$_POST,	$name, $default);	}
	
	private function set_session_array($name, $value)
	{
		$this->set_session($name, array(TSM_V_INCOMED => $value[TSM_V_INCOMED], TSM_V_VALUE => is_null($value[TSM_V_VALUE]) ? null : implode(TSM_DELIMITER, $value[TSM_V_VALUE])));
	}
	
	private function get_ass_def_array(&$ass_array, $value_name, $default, $is_session = false)
	{
		$value = null;
		$incomed = array_key_exists($value_name, $ass_array);
		//dprint($value_name, $incomed);
		if($incomed)
		{
			$value = $ass_array[$value_name] != '' ? explode(TSM_DELIMITER, $ass_array[$value_name]) : null;
			if(!$is_session) { unset($ass_array[$value_name]); }
        }
		if(!$is_session)
		{
			foreach($ass_array as $v_name => $v_value)
			{
				if(preg_match("/^{$value_name}_.*/", $v_name) && eregi(TSM_TRUE_REGEXP, $v_value))
				{
					if(!$incomed) { $incomed = true; }
					$value[] = substr($v_name, strlen("{$value_name}_"));
					unset($ass_array[$v_name]);
				}
			}
		}
		return $incomed ? array(TSM_V_INCOMED => true, TSM_V_VALUE => $value) : $default;
	}
	
	private function get_session_array	($name, $default) { return $this->get_ass_def_array(&$_SESSION,	$name, $default, true); }
	private function get_get_array		($name, $default) { return $this->get_ass_def_array(&$_GET,		$name, $default); }
	private function get_post_array		($name, $default) { return $this->get_ass_def_array(&$_POST,	$name, $default); }
	
	private function first_prepare_value($value) { return array(TSM_V_INCOMED => false, TSM_V_VALUE => $value); }
	
    public function prepare_variable($name, $type = TSM_STRING, $default = null, $store = true, $source = TSM_POST_GET)
	{
		$value = null;
		$default = $this->first_prepare_value($default);
		switch($source)
		{
			case TSM_POST_GET:	$value = $this->get_post($name, $this->get_get ($name, $default));	break;
			case TSM_GET_POST:	$value = $this->get_get	($name, $this->get_post($name, $default));	break;
			case TSM_POST:		$value = $this->get_post($name, $default);							break;
			case TSM_GET:		$value = $this->get_get	($name, $default);							break;
        }
		if($store)
		{
			if(!$value[TSM_V_INCOMED]) { $value = $this->get_session($name, $default); }
			else { $this->set_session_value($name, $value); }
        }
		$this->storage[$name][TSM_DATA_TYPE] = $type;
		$this->storage[$name][TSM_IS_ARRAY] = false;
		switch($type)
		{
			case TSM_INT:		$value[TSM_V_VALUE] = (int)$value[TSM_V_VALUE];												break;
			case TSM_BOOL:		$value[TSM_V_VALUE] = eregi(TSM_TRUE_REGEXP, $value[TSM_V_VALUE]);							break;
			case TSM_IP:		if(isint($value[TSM_V_VALUE])) { $value[TSM_V_VALUE] = int2ip($value[TSM_V_VALUE]); }		break;
			case TSM_SUBNET:	if(isint($value[TSM_V_VALUE])) { $value[TSM_V_VALUE] = int2subnet($value[TSM_V_VALUE]); }	break;
			case TSM_SDATE :	$value[TSM_V_VALUE] = sdate2date($value[TSM_V_VALUE]);										break;
			case TSM_STRING:																					
			default:																										break;
        }
		$this->storage[$name][TSM_DATA] = $value;
    }
	
	public function prepare_variable_batch($names, $type, $defaults, $store = true, $source = TSM_POST_GET)
	{
		foreach($names as $i => $name)
		{
			$this->prepare_variable($name, $type, $defaults[$i], $store, $source);
        }
    }
	
	public function prepare_array($name, $type = TSM_STRING, $default = null, $store = true, $source = TSM_POST_GET)
	{
		// name=val,val1,val2,val3... или name_val=tue name_val1=false....
		$value = null;
		$default = $this->first_prepare_value($default);
		switch($source)
		{
			case TSM_POST_GET:	$value = $this->get_post_array	($name, $this->get_get_array ($name, $default));	break;
			case TSM_GET_POST:	$value = $this->get_get_array	($name, $this->get_post_array($name, $default));	break;
			case TSM_POST:		$value = $this->get_post_array	($name, $default);									break;
			case TSM_GET:		$value = $this->get_get_array	($name, $default);									break;
        }
		//dvdump($name, $value);
		if($store)
		{
			if(!$value[TSM_V_INCOMED]) { $value = $this->get_session_array($name, $default); }
			else { $this->set_session_array($name, $value); }
		}
		$this->storage[$name][TSM_DATA_TYPE] = $type;
		$this->storage[$name][TSM_IS_ARRAY] = true;
		if(is_array($value[TSM_V_VALUE]))
		{
			switch($type)
			{
				case TSM_INT:		foreach($value[TSM_V_VALUE] as &$v) { $v = (int)$v; }							break;
				case TSM_BOOL:		foreach($value[TSM_V_VALUE] as &$v) { $v = eregi(TSM_TRUE_REGEXP, $v); }		break;
				case TSM_IP:		foreach($value[TSM_V_VALUE] as &$v) { if(isint($v)) { $v = int2ip($v); } }		break;
				case TSM_SUBNET:	foreach($value[TSM_V_VALUE] as &$v) { if(isint($v)) { $v = int2subnet($v); } }	break;
				case TSM_SDATE :	foreach($value[TSM_V_VALUE] as &$v) { $v = sdate2date($v); }					break;
				case TSM_STRING:																					
				default:																							break;
			}
			sort($value[TSM_V_VALUE]);
			$this->storage[$name][TSM_DATA] = $value;
		}
		else
		{
			$this->storage[$name][TSM_DATA] = $default;
		}
    }
	
	public function value($name)
	{
		return $this->storage[$name][TSM_DATA][TSM_V_VALUE];
    }
	
	public function get_link($script_name = 'index.php')
	{
		$result = array();
		foreach($this->storage as $name => $values)
		{
			$value = null;
			if($values[TSM_IS_ARRAY])
			{
				if(count($values[TSM_DATA][TSM_V_VALUE]))
				{
					$tmp_array = array();
					switch($values[TSM_DATA_TYPE])
					{
						case TSM_BOOL:		foreach($values[TSM_DATA][TSM_V_VALUE] as $v) { $tmp_array[] = $v ? 't' : 'f'; }	break;
						case TSM_IP:		foreach($values[TSM_DATA][TSM_V_VALUE] as $v) { $tmp_array[] = ip2int($v); }		break;
						case TSM_SUBNET:	foreach($values[TSM_DATA][TSM_V_VALUE] as $v) { $tmp_array[] = subnet2int($v);  }	break;
						case TSM_SDATE :	foreach($values[TSM_DATA][TSM_V_VALUE] as $v) { $tmp_array[] = sdatea2sdate($v); }	break;
						case TSM_INT:
						case TSM_STRING:
						default:			$tmp_array = $values[TSM_DATA][TSM_V_VALUE];										break;
					}
					$value = implode(TSM_DELIMITER, $tmp_array);
				}
			}
			else
			{
				switch($values[TSM_DATA_TYPE])
				{
					case TSM_BOOL:		$value = $values[TSM_DATA][TSM_V_VALUE] ? 't' : 'f';		break;
					case TSM_IP:		$value = ip2int($values[TSM_DATA][TSM_V_VALUE]);			break;
					case TSM_SUBNET:	$value = subnet2int($values[TSM_DATA][TSM_V_VALUE]);		break;
					case TSM_SDATE :	$value = sdatea2sdate($values[TSM_DATA][TSM_V_VALUE]);		break;
					case TSM_INT:
					case TSM_STRING:
					default:			$value = $values[TSM_DATA][TSM_V_VALUE];					break;
				}
            }
			if(!is_null($value))
			{
				$result[] = "{$name}={$value}";
            }
        }
		sort($result);
		return "{$script_name}?" . implode('&', $result);
    }
}
?>
