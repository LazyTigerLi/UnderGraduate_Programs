`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    17:48:41 05/31/2018 
// Design Name: 
// Module Name:    mux2_1_1bit 
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
module mux2_1_1bit(
	input data1,
	input data2,
	input choice,
	output reg data
    );
	 always @ (*)
	 begin
			if(choice == 1'b0)data = data1;
			else data = data2;
	 end
endmodule
