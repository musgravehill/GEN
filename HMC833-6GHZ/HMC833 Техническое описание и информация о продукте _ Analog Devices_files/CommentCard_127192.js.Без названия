//Comment Card Javascript Template for Universal Code

iperceptions_127192 = window.iperceptions || {};

//BEGIN Configuration variables
iperceptions_127192.imgSizeOut = [68, 68];
iperceptions_127192.imgSizeOver = [180, 68];
iperceptions_127192.blockMobile = true;
iperceptions_127192.surveyID = 127192;
iperceptions_127192.langIDArray = [1];
iperceptions_127192.posXArray = ['right'];
iperceptions_127192.posYArray = ['bottom'];
iperceptions_127192.winWidth = 676;
iperceptions_127192.winHeight = 676;

//Reserved for advanced use.  Do not modify these.
iperceptions_127192.iconBrandID = null;
iperceptions_127192.logoBrandID = null;
iperceptions_127192.siteID = null;
iperceptions_127192.imgOutArray = ['data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAEQAAABECAYAAAA4E5OyAAAAAXNSR0IArs4c6QAAAARnQU1BAACxjwv8YQUAAAAgY0hSTQAAeiYAAICEAAD6AAAAgOgAAHUwAADqYAAAOpgAABdwnLpRPAAABEpJREFUeF7tnDuKFFEUhmcLbsEtuAW34BbcgltwC0ZGJmJkJiJioIEMCBqIoGgygaAGEwhiUM7Xzj/8fafq3lPPrkcXNNN0V1ff891z/vOoZk5OjseRQCOBhy+qIx0RAMYRyCUNwTgCuQDiMDYPJIWxaSB1MDYLpAnGJoHkYGwOSAnGpoBEYGwGSBTGJoC0gbF6IG1hrBpIFxirBZKBcfvZu/9N3GYKMzP0xqNXe4Y/+HRW3X//bUNADAbGn/44rwTlzssPFcd2gCSeAQwOQdkWkBo9wDMcioB8Of9dPf76vbr7+mN188mb6+Gz+JFZRiBTKDuXSQ6gAedKZBcNJJBaHQos0Ba8RRrjOrPsEWIGBqGA0UqxDoWQSbPPXgpepIdkYNw7/Vz9+vN3Fxj8lU6k4dMIZXFAMjAknIQB6RU4vvuCci1M/JqLAlLQDLIHR5o9gMN7wAHKOkImIKBKs16eA0fhky3Zdf1FeEghTASAEPFiTB6BlgBrHUAaYOD2z89+XlUVpFEXTiAQKvKavVpjsc1dZuEyFBDKKoLioCCWiusyu91CanVDU0/BYMKIrHPr6dtYqMxaQwoCSihwYLDqDp7LU8gotX1KQJjnp6mBRWv30QwgKCQIIwloNrUuRkMuF5oaw26z6xiM8XpfWYW/hAYw8JTOMGY1QjQYGI5AogXAUCgopaQzDn89lFpn7yG2QO20OlP0goaMtMnDZxwYr9SKsPbyjNmIqnmGtMChqJ7QzmM0gDjCtUVAl+YxD7GFaufroLD7HgrSjuJ8tA2Ig3tIslgZ6fVFGj6Cgr60LriicA6SdxsWV4JCqHiF2ltA69YxOZAGGEqtnlHqwof0W7zZFPWGgwPJwACEiiyFRF34KB2P4h2T1iGZXVMp7lmjLnw6l+NtPGaSkAn2JmlX6k0bFehoXuHrGx1IYHc0CyVkvDtFL4AyephMBqQAwytL6QZQeK75aKtZRgB+0ctG85CCZiibkEYVDi6mKt2LBgwBYXQPCQgoHiAv0GwD4wkZv8m0fCCFHcMzVIqrEsUzyCCTZJGSRw0aMgHNwBsAQnElvdDzUXqTEoD0/cGAZL7YxVOdqmtE+LcbbY3rcv4gQBq+mJBQLSFv8IJLv9PQ6G8dIdMAQ/NOzTPkGQDyG9K6q9Z6Ot5l9yOf6eUhl1+A8ey2h4Y8wVMqcNALHpzLOT4jnTyjDNrcGQy15C6KfqtAN5MAoVQ7yLgvsuNtz+nkITUw0vLatcJLcsFbD5ACDLRAxsp49ANA8ppJe5NRPSQAw28UpTekD1aOt4ESDhm7qM86dY/EX0sbMrQD7xh10tXG6Ny5ISA1F0ihqFmbvDsdCoSuUwSS+UKHMtoUfGiDS9fLAil9+OL9uvCZRT0RWHvtOrP/GCB40VVBqQUSBOGEVwPlGpAOMATG5xuLDZs9ID1gCMAsOtY+dlwB6XORNX12B2RNBvW05R+KtdVx9I5+dgAAAABJRU5ErkJggg=='];
iperceptions_127192.imgOverArray = ['data:image/png;base64,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'];
//END Configuration variables.


iperceptions_127192.isMobile = (iPerceptions.Wrapper.device=="mobile"? true : (iPerceptions.Wrapper.device=="tablet"? true : false));

//API function to add floating icon
iperceptions_127192.ipeIcon = function(config) {
    config = config || {};
    if(iperceptions_127192.launchedIcon) {
        return;
    }
    iperceptions_127192.launchedIcon = true;

    var langIDIndex = 0;
    for (var i = 0; i < iperceptions_127192.langIDArray.length; i++)
    {
        if ( iperceptions_127192.langIDArray[i] == config.langID )
        {
            langIDIndex = i;
            break;
        }
    }

    var _pos_x = config.pos_x || iperceptions_127192.posXArray[langIDIndex], //left|middle|right
        _pos_y = config.pos_y || iperceptions_127192.posYArray[langIDIndex], //'top'|middle|bottom 
        _img_out = config.img_out || iperceptions_127192.imgOutArray[langIDIndex],
        _img_over = config.img_over || iperceptions_127192.imgOverArray[langIDIndex], 
        _size_out = iperceptions_127192.imgSizeOut || [68, 68],
        _size_over = iperceptions_127192.imgSizeOver || [180, 68],
        _size_current = _size_out,
        _id = Math.ceil(Math.random() * 10000),
        _img_id = 'UIF-IMG-' + _id,
		_page = '<img id="' + _img_id + '" ' +
        ' src="' + _img_out + '"' +
        'style="display:none;position:fixed;cursor:pointer;margin:0px!important;padding:0px!important;border:0px!important;' +
        'z-index: 2147483647!important;' +
		(_pos_x == 'right' ? 'text-align:right!important;' : 'text-align:left!important;') +
		(_pos_y == 'bottom' ? 'vertical-align:bottom!important;' : 'vertical-align:top!important;') +
        'background-color:transparent!important;width:auto; height:auto;"/>', 
		_img, _is_ie, _icon_timer,
      
        //Generic method to attach an event to a DOM element
        domAddEvent = function (target, eventName, handlerFn) {
            if (target.addEventListener)
                target.addEventListener(eventName, handlerFn, true);
            else if (target.attachEvent)
                target.attachEvent("on" + eventName, handlerFn);
            else
                target["on" + eventName] = handlerFn;
        },

        ipeSetPos = function(pos_x, pos_y) {
            var divW = _size_current[0],
                divH = _size_current[1],
                viewsize = iperceptions_127192.viewSize(),
                bodyWidth, bodyHeight, winW, winH;

            //Our default viewsize calculation sometimes to include the scrollbar.  document.documentElement.client[Width|Height] do not include scrollbar.
            bodyWidth = document.documentElement.clientWidth;
            if (bodyWidth > 0 && bodyWidth < viewsize[0]) {
                viewsize[0] = bodyWidth;
            }

            bodyHeight = document.documentElement.clientHeight;
            if (bodyHeight > 0 && bodyHeight < viewsize[1]) {
                viewsize[1] = bodyHeight;
            }

            winW = parseInt((viewsize[0] - divW));
            winH = parseInt((viewsize[1] - divH));
               
            if (pos_x == 'left') { _img.style.left = '0px'; }
            else if (pos_x == 'right') { _img.style.right = '0px'; }
            else if(winW >= 0) { _img.style.left = (winW / 2 >> 0) + 'px'; }
            if (pos_y == 'top') { _img.style.top = '0px'; }
            else if (pos_y == 'bottom') { _img.style.bottom = "0px"; }
            else if(winH >= 0) { _img.style.top = (winH / 2 >> 0) + 'px'; }
        },

        isIE = function() {
            var ua = navigator.userAgent.toLowerCase(),
            isOpera = (ua.indexOf('opera') != -1);
            return ua.indexOf('msie') != -1 && !isOpera && (ua.indexOf('webtv') == -1);
        },

        IEMajorVersion = function() {
            var m = /MSIE\s*(\d*)/.exec(navigator.userAgent),
                v = (m && m[1]) || -1;
            if(isNaN(parseInt(v, 10))) {
                v = -1;
            }
            return v;
        },

		ipeIconComplete = function() {
            _img = document.getElementById(_img_id);
            _img.style.display = "block";
            ipeSetPos(_pos_x, _pos_y);

            domAddEvent(window, 'resize', function () { ipeSetPos(_pos_x, _pos_y); });
            domAddEvent(window, 'scroll', function () { ipeSetPos(_pos_x, _pos_y); }); 
            domAddEvent(_img, "mouseout", function () {
                    _img.src = _img_out;
                    _size_current = _size_out;
                });
            domAddEvent(_img, "mouseover", function () {
                    _img.src = _img_over;
                    _size_current = _size_over;
            });
            domAddEvent(_img, "click", function (e) {
                iperceptions_127192.ipeCC(config);
            });
        };

    _is_ie = isIE();

    //Now blocking IE6 and mobile devices
    if(_is_ie && IEMajorVersion() < 8) {
        return;
    }
    if(iperceptions_127192.blockMobile && iperceptions_127192.isMobile) {
        return;
    }

    _icon_timer = setInterval(function() {
        if(document.readyState === "complete") {
            if (_is_ie && IEMajorVersion() < 10) {
                document.body.insertAdjacentHTML("beforeEnd", _page);
            } else {
                _dL = document.createElement("div");
                _dL.innerHTML = _page;
                document.body.appendChild(_dL);
            }
            clearInterval(_icon_timer);
            ipeIconComplete();
        }  
    }, 250);
}


//API function to launch comment card
iperceptions_127192.ipeCC = function(config) {
    var config = config || {};
    config.surveyID = config.surveyID || iperceptions_127192.surveyID;

    iperceptions_127192.launch(config);
}

iperceptions_127192.viewSize = function() {
    var width = 0, height = 0;
    if (typeof (window.innerWidth) == 'number') {
        width = window.innerWidth;
        height = window.innerHeight;
    } else if (document.documentElement && (document.documentElement.clientWidth || document.documentElement.clientHeight)) {
        width = document.documentElement.clientWidth;
        height = document.documentElement.clientHeight;
    } else if (document.body && (document.body.clientWidth || document.body.clientHeight)) {
        width = document.body.clientWidth;
        height = document.body.clientHeight;
    }
    return [width, height];
}

iperceptions_127192.launch = function(config) {
    config.langID = config.langID || iperceptions_127192.langID;

    var RC = function(n){var nEQ= n+'='; var ca= document.cookie.split(';');for(var i=0;i < ca.length;i++) {var c= ca[i];while (c.charAt(0)==' ') c= c.substring(1,c.length);if (c.indexOf(nEQ) == 0) return c.substring(nEQ.length,c.length);} };

    var name = "IPerceptions_comment_" + config.surveyID;
    var w = 0, h = 0, url = "", l = 0, t = 0, vs = iperceptions_127192.viewSize(), 
        sdfc = "c6c349ef" + "-" + config.surveyID + "-" + "c00a4823-bf75-48a3-845a-66c5422eeafe";
    w = iperceptions_127192.winWidth || 676;
    h = iperceptions_127192.winHeight || 676;
    l = Math.max((vs[0] - w) / 2, 0);
    t = Math.max((vs[1] - h) / 2, 0);
    url = window.location.protocol + "//ips-invite.iperceptions.com/WebValidator.aspx?lID=" + config.langID + "&sdfc=" + sdfc + "&source=91787&destination=commentcard&referrer=" + encodeURIComponent(window.location.href) + (iperceptions_127192.logoBrandID ? "&brandID=" + iperceptions_127192.logoBrandID : "" ) + (iperceptions_127192.siteID ? "&siteID=" + iperceptions_127192.siteID : "" );
 
    var features = "width=" + w + ",height=" + h + ", left=" + l + ", top=" + t + ", resizable=yes, toolbar=no, location=no, directories=no, status=no, menubar=no, scrollbars=yes, copyhistory=no";

    window.open(url, name, features);
}

var ipeCC = [iperceptions_127192];
