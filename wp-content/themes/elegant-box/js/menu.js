/*
Author: mg12
Update: 2008/08/11
Author URI: http://www.neoease.com/
*/
(function() {

var Class = {
	create: function() {
		return function() {
			this.initialize.apply(this, arguments);
		}
	}
}

var GhostlyMenu = Class.create();
GhostlyMenu.prototype = {

	initialize: function(target, align, sub) {
		this.obj = cleanWhitespace(target);
		this.align = align || 'left';
		this.sub = sub || -1;

		this.menu = this.obj.childNodes;
		if (this.menu.length < 2) { return; }

		this.title = this.menu[0];
		this.body = this.menu[1];

		cleanWhitespace(this.body).firstChild.getElementsByTagName('a')[0].className += 'first';

		setStyle(this.body, 'visibility', 'hidden');
		setStyle(this.body, 'display', 'block');

		addListener(this.obj, 'mouseover', bind(this, this.activate), false);
		addListener(this.obj, 'mouseout', bind(this, this.deactivate), false);
	},

	activate: function() {
		if(this.sub == 1) {
			var pos = currentOffset(this.title);
			var top = pos[1] + 3;
			var left = getWidth(this.body) - 6;
		} else {
			var pos = cumulativeOffset(this.title);
			var top = pos[1] + getHeight(this.title);
			var left = pos[0] - 1;
		}

		if(!/current/.test(this.title.className)) {
			this.title.className += ' current';
		}

		setStyle(this.body, 'left', left + 'px');
		setStyle(this.body, 'top', top + 'px');
		setStyle(this.body, 'visibility', 'visible');
	},

	deactivate: function(){
		this.title.className = this.title.className.replace('current', '');
		var thismenu = this;
		var tid = setInterval( function() {
			clearInterval(tid);
			if (!/current/.test(thismenu.title.className)) {
				setStyle(thismenu.body, 'visibility', 'hidden');
			}
			return false;
		}, 400);
	}
}

$A = function(iterable) {
	if(!iterable) {
		return [];
	}
	if(iterable.toArray) {
		return iterable.toArray();
	} else {
		var results = [];
		for(var i = 0; i < iterable.length; i++) {
			results.push(iterable[i]);
		}
		return results;
	}
}

bind = function() {
	var array = this.$A(arguments);
	var func = array[array.length - 1];
	var _method = func, args = array, object = args.shift();
	return function() {
		return _method.apply(object, args.concat(array));
	}
}

getHeight = function(element) {
	return element.offsetHeight;
}

getWidth = function(element) {
	return element.offsetWidth;
}

setStyle = function(element, key, value) {
	element.style[key] = value;
}

getStyle = function(element, key) {
	return element.style[key];
}

cleanWhitespace = function(list) {
	var node = list.firstChild;
	while (node) {
		var nextNode = node.nextSibling;
		if(node.nodeType == 3 && !/\S/.test(node.nodeValue)) {
			list.removeChild(node);
		}
		node = nextNode;
	}
	return list;
}

currentOffset = function(element) {
	var valueT = element.offsetTop  || 0;
	var valueL = element.offsetLeft || 0;
	return [valueL, valueT];
}

cumulativeOffset = function(element) {
	var valueT = 0, valueL = 0;
	do {
		valueT += element.offsetTop  || 0;
		valueL += element.offsetLeft || 0;
		element = element.offsetParent;
	} while (element);
	return [valueL, valueT];
}

addListener = function(element, name, observer, useCapture) {
	if(element.addEventListener) {
		element.addEventListener(name, observer, useCapture);
	} else if(element.attachEvent) {
		element.attachEvent('on' + name, observer);
	}
}

function loadMenus() {
	var menubar = document.getElementById('navigation');
	if (menubar) {
		var list = menubar.getElementsByTagName('ul');
		for (var i = 0; i < list.length; i++) {
			var menu = list[i].parentNode;
			if(menu.parentNode === menubar) {
				new GhostlyMenu(menu, 'left');
			} else {
				new GhostlyMenu(menu, 'left', 1);
				menu.firstChild.className += ' subtitle';
			}
		}
	}
}

if (document.addEventListener) {
	document.addEventListener("DOMContentLoaded", loadMenus, false);

} else if (/MSIE/i.test(navigator.userAgent)) {
	document.write('<script id="__ie_onload_for_elegantbox" defer src="javascript:void(0);"></script>');
	var script = document.getElementById("__ie_onload_for_elegantbox");
	script.onreadystatechange = function() {
		if (this.readyState == 'complete') {
			loadMenus();
		}
	}

} else if (/WebKit/i.test(navigator.userAgent)) {
	var _timer = setInterval( function() {
		if (/loaded|complete/.test(document.readyState)) {
			clearInterval(_timer);
			loadMenus();
		}
	}, 10);

} else {
	window.onload = function(e) {
		loadMenus();
	}
}

})();