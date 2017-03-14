function progress_show_text(text)
{
	var pr = document.getElementById('progress_text');
	pr.innerHTML = text;
}

function progress_show_gauge(text, done, max, current)
{
	progress_show_text(text + "<br />" + done + "%&nbsp;" + "[" + current + "/" + max + "]" + "<div class='progress_todo'><div class='progress_done' style='width:" + done + "%;'></div></div>");
}

function hide()
{
	var pr = document.getElementById('progress');
	pr.style.visibility = 'collapse';
}
