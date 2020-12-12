$(document).ready(function () {
	// Обрабатываем нажатие на все ссылки
	$("a[id^='ratestar-']").click( RatingStarSend );
	// Рейтинг +1/-1
	$("a[id^='rateone-']").click( RatingOneSend );
	
});
// Функция отправки рейтинга
function RatingStarSend() {
	
	var identifier = $(this).attr( "id" );
	var reg_id = /ratestar\-(\d+)\-(\d+)\-(\d+)\-(\d+)/;
	// Распарсиваем идентификатор
	// arr_id[1] - ID модуля
	// arr_id[2] - ID категории
	// arr_id[3] - ID элемента
	// arr_id[4] - Оценка
	var arr_id = reg_id.exec( identifier );
	
	// Идентификатор рейтинга
	var ratingid = $("#ratingstar-" + arr_id[1] + "-" + arr_id[2] + "-" + arr_id[3]);
	// Устанавливаем ширину
	//ratingid.attr( "style","width:75%;" );
	// Идентификатор голосов
	var ratingvotesid = $("#ratingstar-votes-" + arr_id[1] + "-" + arr_id[2] + "-" + arr_id[3]);
	
	//alert( "cid: " + arr_id[1] + ", itemid: " + arr_id[2] + ", rate:" + arr_id[3] );
	
	
	// Выполняем запрос к серверу
	$.post( rating_config_ajaxurl,
	{
		op: "starsend",  // Опция отправки данных
		mid: arr_id[1],  // ID модуля
		cid: arr_id[2],  // ID категории
		itemid: arr_id[3], // ID элемента
		rate: arr_id[4] // Оценка
	}, function( result ) {
		// Массив Сообщений
		var rate_messages = result["messages"];
		// Сообщение
		var rate_message = "";
		// Если есть сообщения
		if( rate_messages.length > 0 ) {
			// Разбиваем элементы массива в строку
			rate_message = rate_messages.join( "<br />" );
		// Если небыло сообщений
		} else {
			rate_message = rating_lang_ratesuccessupdate;
		}
		
		// Сообщение
		RatingMessage( rate_message )
		
		// Рейтинг
		if( result["rating"] ) {
			// Ширина
			var width = result["rating"] * 10;
			ratingid.attr( "style", "width:" + width + "%;" );
		}
		// Число голосов
		if( result["votes"] && result["txtvotes"] ) {
			// Устанавливаем число голосов
			ratingvotesid.text( result["txtvotes"] );
			// Устанавливаем title
			ratingvotesid.attr( "title", result["votes"] );
		}
		
		
	}, "json" );
	
	return false;
}

function RatingOneSend() {
	// ID ссылки
	var identifier = $(this).attr( "id" );
	//
	var reg_id = /rateone\-(\d+)\-(\d+)\-(\d+)\-(yes|no)/;
	// Распарсиваем идентификатор
	// arr_id[1] - ID модуля
	// arr_id[2] - ID категории
	// arr_id[3] - ID элемента
	// arr_id[4] - yes или no
	var arr_id = reg_id.exec( identifier );
	
	// Идентификатор голосов
	var ratingyesvotesid = $("div#ratingone-" + arr_id[1] + "-" + arr_id[2] + "-" + arr_id[3] + " span.rating-one-votes-yes");
	var ratingnovotesid = $("div#ratingone-" + arr_id[1] + "-" + arr_id[2] + "-" + arr_id[3] + " span.rating-one-votes-no");
	
	
	
	// Выполняем запрос к серверу
	$.post( rating_config_ajaxurl,
	{
		op: "onesend", // Опция отправки данных
		mid: arr_id[1], // ID модуля
		cid: arr_id[2], // ID категории
		itemid: arr_id[3], // ID элемента
		rateyn: arr_id[4] // Оценка
	}, function( result ) {
		
		// Массив сообщений
		var rate_messages = result["messages"];
		// Сообщение
		var rate_message = "";
		// Если есть сообщения
		if( rate_messages.length > 0 ) {
			// Разбиваем элементы массива в строку
			rate_message = rate_messages.join( "<br />" );
		// Если небыло сообщений
		} else {
			rate_message = rating_lang_ratesuccessupdate;
		}
		
		// Сообщение
		RatingMessage( rate_message )
		
		// Число голосов
		if( result["txtyesvotes"] ) {
			// Устанавливаем число голосов
			ratingyesvotesid.text( result["txtyesvotes"] );
			// Устанавливаем title
			ratingyesvotesid.attr( "title", result["yesvotes"] );
		}
		//
		if( result["txtnovotes"] ) {
			// Устанавливаем число голосов
			ratingnovotesid.text( result["txtnovotes"] );
			// Устанавливаем title
			ratingnovotesid.attr( "title", result["novotes"] );
		}
		
	}, "json" );
	
	return false;
}

// Сообщения
function RatingMessage( rate_message ) {
	//
	$.jGrowl( rate_message, {  header: rating_lang_rateinfo, life:5000, position: "center", speed: "slow" });
	
}