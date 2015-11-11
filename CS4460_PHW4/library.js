function drawBars(data) {
  /**
   * draws bars yo
   *
   **/
  bar.selectAll(".bar")
      .data(data)
      .enter().append("rect")
      .attr("class", "bar")
      .attr("fill", "white")
      .attr("x", function(d) { return x1(d.Manufacturer); })
      .attr("width", x1.rangeBand())
      .attr("y", function(d) { return y1(d.Avg); })
      .attr("height", function(d) { return height1 - y1(d.Avg); })
      .on("click", changeOpacity);
}



//takes from the array in index.html
function drawXAxis(){
  bar.append("g")
      .attr("class", "x axis")
      .attr("transform", "translate(0," + height1 + ")")
      .call(xAxis1)
      .attr("fill", "white")
      .selectAll("text")  
            .style("text-anchor", "end")
            .attr("dx", "-.8em")
            .attr("dy", ".15em")
            .attr("transform", "rotate(-65)" );
}

//draws y axis
function drawYAxis() {
  bar.append("g")
      .attr("class", "y axis")
      .call(yAxis1)
      .attr("fill", "white")
      .append("text")
      .attr("transform", "rotate(-90)")
      .attr("y", 6)
      .attr("dy", ".71em")
      .style("text-anchor", "end")
      .text("Avg. Calories");
}

//called when a dot is clicked, changes colors of
//bars whose average calories is higher
//than the dots
function changeBars() {
//find calories of clicked circle
  datum = d3.select(this).data()[0];

  if (datum != null)
    var c = datum.Calories;

//Change the color of bars whose average Calories count higher than that of the clicked circle
  bar.selectAll(".bar") //bar is the bar chart
      .filter(function(d) { return d.Avg > c } )
        .transition()
        .duration(function(d) { return 1000; } )
        .delay(function(d) { return 100; })
        .style("fill", "green");

  bar.selectAll(".bar") //bar is the bar chart
    .filter(function(d) { return d.Avg <= c } )
        .transition()
        .duration(function(d) { return 1000; } )
        .delay(function(d) { return 100; })
        .style("fill", "white");
}

//called when a bar is clicked, changes the 
//opacity of the dots that are the same manufacturer
//as the clicked bar
function changeOpacity(datum) {
//find manufacturer
  if (datum != null)
    var m = datum.Manufacturer;
//change all others to opacity of 25%
  svg.selectAll(".dot")
      .filter(function(d) { return d["Manufacturer"] != m } )
        .transition()
        .duration(function(d) { return 1000; } )
        .delay(function(d) { return 100; })
        .attr("r", sizeForCircle)
        .attr("cx", xMap)
        .attr("cy", yMap)
        .style("opacity", .25);
//make the selected man. to full opacity
  svg.selectAll(".dot")
      .filter(function(d) { return d["Manufacturer"] == m } )
        .transition()
        .duration(function(d) { return 1000; } )
        .delay(function(d) { return 100; })
        .attr("r", sizeForCircle)
        .attr("cx", xMap)
        .attr("cy", yMap)
        .style("opacity", 1);
}
