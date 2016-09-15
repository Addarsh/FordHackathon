var mongoose = require('mongoose');
var Schema = mongoose.Schema;

// create a schema
var spotSchema = new Schema({
  	id : { type: Number, required: true, unique: true },
  	status : String,
	type : String,
	lat : Number,
	lng : Number,
  	updated_at: Date
});

spotSchema.pre('save', function(next) {
	var currentDate = new Date();
	this.updated_at = currentDate;
	next();
});

var ParkItSpot = mongoose.model('ParkItSpot', spotSchema);

module.exports = ParkItSpot;
