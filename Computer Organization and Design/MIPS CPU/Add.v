`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    19:30:27 04/25/2018 
// Design Name: 
// Module Name:    Add 
// Project Name: 
// Target Devices: 
// Tool versions: 
// Description: 
//
// Dependencies: 
//
// Revision: 
// Revision 0.01 - File Created
// Additional Comments: 
//
//////////////////////////////////////////////////////////////////////////////////
module Add(
	input [31:0] data1,
	input [31:0] data2,
	output reg [31:0] result
	);
	always @ (data1 or data2)
	begin
		result = data1 + data2;
	end
endmodule
