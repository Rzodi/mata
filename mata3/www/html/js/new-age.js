(function($) {
  "use strict"; // Start of use strict

  // Smooth scrolling using jQuery easing
  $('a.js-scroll-trigger[href*="#"]:not([href="#"])').click(function() {
    if (location.pathname.replace(/^\//, '') == this.pathname.replace(/^\//, '') && location.hostname == this.hostname) {
      var target = $(this.hash);
      target = target.length ? target : $('[name=' + this.hash.slice(1) + ']');
      if (target.length) {
        $('html, body').animate({
          scrollTop: (target.offset().top - 48)
        }, 1000, "easeInOutExpo");
        return false;
      }
    }
  });

  // Closes responsive menu when a scroll trigger link is clicked
  $('.js-scroll-trigger').click(function() {
    $('.navbar-collapse').collapse('hide');
  });

  // Activate scrollspy to add active class to navbar items on scroll
  $('body').scrollspy({
    target: '#mainNav',
    offset: 54
  });

  // Collapse Navbar
  var navbarCollapse = function() {
    if ($("#mainNav").offset().top > 100) {
      $("#mainNav").addClass("navbar-shrink");
    } else {
      $("#mainNav").removeClass("navbar-shrink");
    }
  };
  // Collapse now if page is not at top
  navbarCollapse();
  // Collapse the navbar when page is scrolled
  $(window).scroll(navbarCollapse);

  $('#submitBtn').on('click',  function(){
	ShowLoadingPanel()
    $.ajax( {
      type: "POST",
      url: "solve.cgi",
      data: $('#solve-form').serialize(),
      success: function(response) {
	        console.log(response);
	        if(response.unexpected_error){
	        	alert("Wystąpił nieoczekiwany błąd.");
	        }else if(!response.is_problem_feasible){
	        	alert("Problem jest nierozwiązywalny.");
	        }
	        else{
	        	setTimeout(function (){
		        	LoadResults(response);
					HideLoadingPanel();
				}, 1000);
	        }	
      }
    });	  
  });
    
  $('#enbCounter').on('change', function(){
		AddENBs(this.value);
  });
  
  $('#routerCounter').on('change', function(){
		AddRouters(this.value);
  });
  
  
})(jQuery); // End of use strict

function ShowLoadingPanel(){
    $("body").addClass("loading")
}
function HideLoadingPanel(){
    $("body").removeClass("loading")
}

function AddENBs(quantity){
	$('#enbs').empty()
	for(var i = 0 ; i < quantity; i++){		
		$('#enbs').append(
		'<div class="row mx-md-5">'+	
				'<div class="col-md-6">'+
					'<label class="col-md-12">Położenie X ('+i+' eNodeB)</label>'+
					'<input class="form-control" name="enbs['+i+'].x" type="number" min="0" value="1">'+
				'</div>'+
				'<div class="col-md-6">'+
					'<label class="col-md-12">Położenie Y ('+i+' eNodeB)</label>'+
					'<input class="form-control" name="enbs['+i+'].y" type="number" min="0" value="1">'+
				'</div>'+					
		'</div>'	
		);
	}	
}

function AddRouters(quantity){
	$('#routers').empty()
	for(var i = 0 ; i < quantity; i++){		
		$('#routers').append(
		'<div class="row mx-md-5">'+	
				'<div class="col-md-6">'+
					'<label class="col-md-12">Położenie X ('+i+' router)</label>'+
					'<input class="form-control" name="routers['+i+'].x" type="number" min="0" value="1">'+
				'</div>'+
				'<div class="col-md-6">'+
					'<label class="col-md-12">Położenie Y ('+i+' router)</label>'+
					'<input class="form-control" name="routers['+i+'].y" type="number" min="0" value="1">'+
				'</div>'+					
		'</div>'	
		);
	}	
}

function ValidateEnbRange(number){
	var enbrange = +$('[name="enb_range"]').val();

	var result = (+$('[name="enbs['+ number +'].x"]').val() - enbrange >= 0)
		&& (+$('[name="enbs['+ number +'].x"]').val() + enbrange <= +$('[name="size_x"]').val())
		&& (+$('[name="enbs['+ number +'].y"]').val() - enbrange >= 0)
		&& (+$('[name="enbs['+ number +'].y"]').val() + enbrange <= +$('[name="size_y"]').val());

	if(!result){
		alert("Błąd w ENB numer " + number);
	}

	return result;
}

function ValidateRouterRange(number){
	var routerrange = +$('[name="router_range"]').val();

	var result = (+$('[name="routers['+ number +'].x"]').val() - routerrange >= 0)
		&& (+$('[name="routers['+ number +'].x"]').val() + routerrange <= +$('[name="size_x"]').val())
		&& (+$('[name="routers['+ number +'].y"]').val() - routerrange >= 0)
		&& (+$('[name="routers['+ number +'].y"]').val() + routerrange <= +$('[name="size_y"]').val());

	if(!result){
		alert("Błąd w routerze numer " + number);
	}

	return result;
}

	
function LoadResults(response){
    $(".mastsolve").empty().append(
		'<div class="container h-100">'+
			'<div class="row">'+
			  '<div class="col-md-12 my-auto">'+
				'<div class="header-content text-center mx-auto">'+
				  '<h1 class="mb-5">Wyniki:</h1>'+
				'</div>'+
			  '</div>'+         
			'</div>'+
			'<div class="text-center col-md-12">'+
				'<canvas id="grid"></canvas>'+	
				'<div id="table"></div>'+				
				
			'</div>'+
		'</div>'
    	);

    var drawTable = function(){
    	var html = "<table>"+
  						"<tr>" + 
    						"<th>Położenie X</th>" + 
    						"<th>Położenie Y</th>" + 
    						"<th>Wifi</th>" + 
    						"<th>LTE</th>" + 
    						"<th>Minimalne żądanie</th>" + 
    						"<th>Maksymalne żadanie</th>" + 
    					"</tr>";
			for(i=0; i<response.positions.users.length; i++){
				html = html + "<tr>"+
								"<th>" + response.positions.users[i].x + "</th>"+
								"<th>" + response.positions.users[i].y + "</th>"+
								"<th>" + response.positions.users[i].wifi + "</th>"+
								"<th>" + response.positions.users[i].lte + "</th>"+
								"<th>" + response.positions.users[i].min_demand + "</th>"+
								"<th>" + response.positions.users[i].max_demand + "</th>"+								
							"</tr>"
			}

			html = html + "</table>";
    	$('#table').append(html);
    }

	var drawGrid = function(w, h, id) {
	    var canvas = document.getElementById(id);
	    var ctx = canvas.getContext('2d');
	    ctx.canvas.width  = w;
	    ctx.canvas.height = h;
	    ctx.strokeStyle = "#A0A0A0";

	    for (x=0;x<=w;x+=20) {
	        for (y=0;y<=h;y+=20) {
	            ctx.moveTo(x, 0);
	            ctx.lineTo(x, h);
	            ctx.stroke();
	            ctx.moveTo(0, y);
	            ctx.lineTo(w, y);
	            ctx.stroke();
	        }
	    }
	
		for(i=0; i<response.positions.routers.length; i++){
			ctx.beginPath();
			ctx.arc(response.positions.routers[i].x,response.positions.routers[i].y,response.router_range ,0,360);
			ctx.fillStyle = "rgba(89, 183, 104, 0.7)";
			ctx.fill();
			ctx.stroke();	
		}

		for(i=0; i<response.positions.enbs.length; i++){
			ctx.beginPath();
			ctx.arc(response.positions.enbs[i].x,response.positions.enbs[i].y,response.enb_range ,0,360);
			ctx.fillStyle = "rgba(212, 89, 48, 0.7)";
			ctx.fill();
			ctx.stroke();	
		}

		for(i=0; i<response.positions.users.length; i++){
			ctx.beginPath();
			ctx.arc(response.positions.users[i].x,response.positions.users[i].y, 5 ,0,360);
			ctx.fillStyle = "rgba(184, 0, 255, 1)";
			ctx.fill();
			ctx.stroke();	
		}		
	}

    drawGrid(response.size_x, response.size_y, "grid");
    drawTable();
}

