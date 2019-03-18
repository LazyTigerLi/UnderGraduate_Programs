`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    15:48:27 05/28/2018 
// Design Name: 
// Module Name:    mux4_1 
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
module mux4_1(
	input [31:0] data1,
	input [31:0] data2,
	input [31:0] data3,
	input [31:0] data4,
	input [1:0] choice,
	output reg [31:0] result
    );
	 always @ (*)
	 begin
			case(choice)
				2'b00: result = data1;
				2'b01: result = data2;
				2'b10: result = data3;
				2'b11: result = data4;
			endcase
	 end
endmodule
