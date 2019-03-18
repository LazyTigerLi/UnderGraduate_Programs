`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    16:40:47 04/30/2018 
// Design Name: 
// Module Name:    PC 
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
module PC(
	input clk,
	input rst_n,
	input [31:0] new_pc,
	input PCwrite,
	output reg [31:0] pc
    );
	 always @ (negedge clk or negedge rst_n)
	 begin
			if(~rst_n)pc <= 32'h0;
			else if(PCwrite)pc <= new_pc;
			else pc <= pc;
	 end
endmodule
