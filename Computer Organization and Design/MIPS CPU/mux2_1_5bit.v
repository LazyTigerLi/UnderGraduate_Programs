`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    17:50:49 05/31/2018 
// Design Name: 
// Module Name:    mux2_1_5bit 
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
module mux2_1_5bit(
	input [4:0] data1,
	input [4:0] data2,
	input choice,
	output reg [4:0] data
    );
	 always @ (*)
	 begin
			if(choice == 1'b0)data = data1;
			else data = data2;
	 end

endmodule
