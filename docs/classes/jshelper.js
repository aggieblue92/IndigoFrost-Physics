function ExpandCollapse(toExpandOrCollapse)
{
	var t = toExpandOrCollapse;
	if(t.style.display == 'none')
	{
		t.style.display = '';
	}
	else
	{
		t.style.display = 'none';
	}
}