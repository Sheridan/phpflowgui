//Общие
// Убирает пробельные символы слева
function ltrim(str)
{
	var ptrn = /\s*((\S+\s*)*)/;
	return str.replace(ptrn, "$1");
}
// Убирает пробельные символы справа
function rtrim(str)
{
	var ptrn = /((\s*\S+)*)\s*/;
	return str.replace(ptrn, "$1");
}
// Убирает пробельные символы с обоих концов
function trim(str) { return ltrim(rtrim(str)); }

// Закидывает линк из конца страницы к ссылке в меню
function set_link(link)
{
	var a_link = document.getElementById('page_link');
	a_link.href = link;
}

// Селект
function append_to_select(select_id, text)
{
	var select = document.getElementById(select_id);
	var can_append = true;
	for (var i=0; i < select.length; i++)
	{
		if(text == select.options[i].text)
		{
			can_append = false;
			break;
		}
	}
	if(can_append)
	{
		var node = document.createElement('option');
		node.value = text;
		node.appendChild(document.createTextNode(text));
		select.appendChild(node);
	}
}

function append_to_select_from_edit(select_id, edit_id)
{
	var edit = document.getElementById(edit_id);
	var edit_text = trim(edit.value);
	if(edit_text != "")
	{
		append_to_select(select_id, edit_text);
	}
	edit.value = '';
}

function remove_from_select(select_id)
{
	var select = document.getElementById(select_id);
	select.remove(select.selectedIndex);
}

function remove_all_from_select(select_id)
{
	var select = document.getElementById(select_id);
	for (var i=0; i < select.length; i++)
    {
		select.remove(i);
    }
}

function copy_select_to_hidden(select_id, hidden_id)
{
	var select = document.getElementById(select_id);
	var hidden = document.getElementById(hidden_id);
	var text = "";
	for (var i=0; i < select.length; i++)
    {
		text = text + select.options[i].text + ",";
    }
	//alert(hidden.value);
	hidden.value = text.substring(0, text.length-1);
}

// Чекбоксовое
function change_checkbox(elementid, value)
{
	var hn = 'id_hidden_' + elementid;
	var cn = 'id_checkbox_' + elementid;
	var hidden = document.getElementById(hn);
	hidden.value = value;
	var checkbox = document.getElementById(cn);
	checkbox.checked = value == "on";
}

function set_checkbox_changed(elementid)
{
	var hn = 'id_hidden_' + elementid;
	var hidden = document.getElementById(hn);
	change_checkbox(elementid, hidden.value == "off" ? "on" : "off");
	return hidden.value;
}

function set_checkbox_group_changed(self, group)
{
	var value = set_checkbox_changed(self);	
	for (var i in group)
	{
		change_checkbox(group[i], value);
	}
}

// Hide and unhide (hu)
function hu_is_visible(obj)
{
	return obj.style.visibility == 'visible';
}

function change_hu_image(img_ctrl_obj, obj)
{
	if(hu_is_visible(obj))
	{
		img_ctrl_obj.src = 'img/w_hide.png';
		img_ctrl_obj.value = 'Hide';
	}
	else
	{
		img_ctrl_obj.src = 'img/w_show.png';
		img_ctrl_obj.value = 'Show';
	}
}

function hu_element(obj)
{
	obj.style.visibility = hu_is_visible(obj) ? 'collapse' : 'visible';
}

function hu_elementid(elementid, imgctrlid)
{
	var obj = document.getElementById(elementid);
	hu_element(obj);
	change_hu_image(document.getElementById(imgctrlid), obj);
}

function hu_open(elementid)
{
	var obj = document.getElementById(elementid);
	obj.style.visibility = 'visible';
}

function hu_close(elementid)
{
	var obj = document.getElementById(elementid);
	obj.style.visibility = 'collapse';
}

// Меню, дополнительное
function append_to_menu(menuid, caption, targetid)
{
	var menu = document.getElementById(menuid);
	if(menu.innerHTML == '<span></span>') { menu.innerHTML = " "; }
	var text = "<span onclick=\"hu_open('" + targetid + "')\"><img src='img/options.png' alt='Options' />&nbsp;" + caption + "</span>";
	menu.innerHTML = menu.innerHTML + text;
}

