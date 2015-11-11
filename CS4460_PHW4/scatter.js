var margin = {top: 50, right: 0, bottom: 30, left: 0};
var width = 500 - margin.left - margin.right;
var height = 600 - margin.top - margin.bottom;

// pre-cursors
var sizeForCircle = function(d) {
  return 6 * d["Serving Size Weight"];
}

// setup x
var xValue = function(d) { return d["Calories"];}, // data -> value
    xScale = d3.scale.linear().range([0, width]), // value -> display
    xMap = function(d) { return xScale(xValue(d));}, // data -> display
    xAxis = d3.svg.axis().scale(xScale).orient("bottom");

// setup y
var yValue = function(d) { return d["Sugars"];}, // data -> value
    yScale = d3.scale.linear().range([height, 0]), // value -> display
    yMap = function(d) { return yScale(yValue(d));}, // data -> display
    yAxis = d3.svg.axis().scale(yScale).orient("left");

// setup fill color
var cValue = function(d) { return d.Manufacturer;},
    color = d3.scale.category10();

// add the graph canvas to the body of the webpage
var svg = d3.select(".scatterplot").append("svg")
    .attr("width", width + margin.left + margin.right)
    .attr("height", height + margin.top + margin.bottom)
    .append("g")
    .attr("transform", "translate(" + margin.left + "," + margin.top + ")");

// add the tooltip area to the webpage
var tooltip = d3.select("body").append("div")
    .attr("class", "tooltip")
    .style("opacity", 0);

// load data
d3.csv("Cereal.csv", function(error, data) {

  // change string (from CSV) into number format
  data.forEach(function(d) {
    d.Potassium = +d.Potassium;
    d["Sodium"] = +d["Sodium"];
    d.Calories = +d.Calories;
    d["Sugars"] = +d["Sugars"];
//    console.log(d);
  });

  data = data.filter(function(d) { return d.Sodium >= 0 } );

  // don't want dots overlapping axis, so add in buffer to data domain
  xScale.domain([d3.min(data, xValue)-1, d3.max(data, xValue)+1]);
  yScale.domain([d3.min(data, yValue)-1, d3.max(data, yValue)+1]);

  // x-axis
  svg.append("g")
      .attr("class", "x axis")
      .attr("transform", "translate(0," + height + ")")
      .attr("fill", "white")
      .call(xAxis)
    .append("text") //just the calories label
      .attr("class", "label")
      .attr("x", width)
      .attr("y", -6)
      .attr("fill", "white")
      .style("text-anchor", "end")
      .text("Calories");

  // y-axis
  svg.append("g")
      .attr("class", "y axis")
      .attr("fill", "white")
      .call(yAxis)
    .append("text") //just the sugars label
      .attr("class", "label")
      .attr("transform", "rotate(-90)")
      .attr("y", 6)
      .attr("dy", ".71em")
      .attr("fill", "white")
      .style("text-anchor", "end")
      .text("Sugars");

  // draw dots
  svg.selectAll(".dot")
      .data(data)
    .enter().append("circle")
      .attr("class", "dot")
      .attr("r", sizeForCircle)
      .attr("cx", xMap)
      .attr("cy", yMap)
      .style("fill", function(d) { return color(cValue(d));})
      
      .on("click", changeBars) //THIS WILL CHANGE THE OPACITIY ON CLICK

      .on("mouseover", function(d) {

          // show the tool tip
          tooltip.transition()
               .duration(200)
               .style("opacity", .75);

          // fill to the tool tip with the appropriate data
          tooltip.html("<strong>" + d["Cereal Name"] + "</strong><br/>Calories: " + xValue(d)
          + "<br/>Sugars: " + yValue(d) + "mg")
               .style("left", (d3.event.pageX + 5) + "px")
               .style("top", (d3.event.pageY - 28) + "px");

      })
      .on("mouseout", function(d) {
          // hide the tooltip
          tooltip.transition()
               .duration(500)
               .style("opacity", 0);
      });

  // draw legend
  var legend = svg.selectAll(".legend")
      .data(color.domain())
    .enter().append("g")
      .attr("class", "legend")
      .attr("transform", "translate(0,350)");

  // draw legend colored rectangles
  legend.append("rect")
      .attr("x", width - 18)
      .attr("y", function(d, i) { return i*20;})
      .attr("width", 18)
      .attr("height", 18)
      .style("fill", color);

  // draw legend text
  legend.append("text")
      .attr("x", width - 24)
      .attr("y", function(d, i) { return i*20+9;})
      .attr("dy", ".35em")
      .attr("fill", "white")
      .style("text-anchor", "end")
      .text(function(d) { return d;});
});