var express = require('express');
var app = express();
var mongoose = require('mongoose');
mongoose.connect('mongodb://localhost/parkit_db');
var bodyParser = require("body-parser");
app.use(bodyParser.json());

var ParkItSpot = require('./models/ParkItSpot.js');

app.post('/api/createSpot', function(req, res) {
	console.log("POST");
	if(req.body.id < 0 || req.body.id > 110) {
		res.status(500).json({error: 'badId'});
	} else {
		console.log("id: "+req.body.id+" status: "+req.body.status);
		var newSpot = ParkItSpot({
			id : req.body.id,
			status : req.body.status,
			lat : req.body.lat,
			lng : req.body.lng,
			type : req.body.type
		});
		newSpot.save(function(err){
		        if (err) {
		                console.log("Error saving new spot");
		        } else {
		                console.log("New Spot Created!");
		                res.json({ status : 'success' });
		        }
		});
	}
});

app.post('/api/updateSpot', function(req, res) {
	console.log("POST");
	if(req.body.id < 0 || req.body.id > 50) {
		res.status(500).json({error: 'badId'});
	} else {
		console.log("id: "+req.body.id+" status: "+req.body.status);
		ParkItSpot.findOneAndUpdate({ id : req.body.id }, { status : req.body.status }, 
			function(err, spot) {
				if(err || spot == null) {
					console.log(err);
					var newSpot = ParkItSpot({ 
						id : req.body.id, 
						status : req.body.status,
						lat : 37.4081386,
						lng : -122.1480287,
						type : "Undefined"
					});
					newSpot.save(function(err){
						if (err) {
							console.log("Error saving new spot");
						} else {
							console.log("New Spot Created!");
							res.json({ status : 'success' });
						}
					});
				} else {
					console.log("Updated spot "+spot);
					res.json({ status : 'success' });
				}
			});
	}
});

app.get('/api/spotInfo', function (req, res) {
	console.log("GET");
	ParkItSpot.find({}, 
		function(err, spots){	
			if(err) {
				console.log('spotnotfound');
				res.json({ error : 'spotNotFound'});
			} else {
				console.log(spots);	
				res.json(spots);
			}
		});
});

app.listen(3000, function () {
  console.log('Example app listening on port 3000!');
});
