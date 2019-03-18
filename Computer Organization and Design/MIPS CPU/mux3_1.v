`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    15:51:47 05/28/2018 
// Design Name: 
// Module Name:    mux3_1 
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
module mux3_1(
	input [31:0] data1,
	input [31:0] data2,
	input [31:0] data3,
	input [1:0] choice,
	output reg [31:0] result
    );
	 always @ (*)
	 begin
			if(choice == 2'b00)result = data1;
			else if(choice == 2'b01)result = data2;
			else result = data3;
	 end
endmodule
