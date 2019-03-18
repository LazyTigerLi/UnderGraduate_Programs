`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    19:18:52 05/16/2018 
// Design Name: 
// Module Name:    IF_ID 
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
module IF_ID(
	input clk,
	input [31:0] PC,
	input [31:0] instruction,
	input IF_ID_write,
	output reg [31:0] PC_out,
	output reg [31:0] instruction_out
    );
	 parameter bgtz = 6'b000111;
	 parameter beq = 6'h4;
	 parameter bgez = 6'h1;
	 parameter blez = 6'h6;
	 parameter bltz = 6'h1;
	 parameter bne = 6'h5;
	 parameter j = 6'h2;
	 parameter jr = 6'h0;
	 always @ (negedge clk)				//在时钟下降沿更新段间寄存器
	 begin
			if(IF_ID_write)
			begin
				PC_out <= PC;
				instruction_out <= instruction;
				if(instruction_out[31:26] == j || (instruction_out[31:26] == jr && instruction_out[5:0] == 6'h8))
				instruction_out <= 32'hFFFFFFFF;
			end
			else
			begin
				PC_out <= PC_out;
				if(instruction_out[31:26] == bgtz || instruction_out[31:26] == bne ||
				instruction_out[31:26] == beq || instruction_out[31:26] == bgez ||
				instruction_out[31:26] == blez || instruction_out[31:26] == bltz)
				instruction_out <= 32'hFFFFFFFF;
				//原来判断条件写成instruction_out[31:26] == bgtz，有找了好久
				else instruction_out <= instruction_out;
			end
	 end
endmodule
