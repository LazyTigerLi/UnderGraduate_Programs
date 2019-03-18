`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    18:52:18 04/04/2018 
// Design Name: 
// Module Name:    REG_FILE 
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
module REG_FILE(
	 input clk,
	 input rst_n,
	 input [4:0] rAddr1,
	 input [4:0] rAddr2,
	 input [4:0] wAddr,
	 input [31:0] wDin,
	 input wEna,
	 output reg [31:0] rDout1,
	 output reg [31:0] rDout2
    );
	 reg [31:0] reg_file [31:0];
	 integer i;
	 always @ (posedge clk or negedge rst_n)
	 begin
			if(~rst_n)
			begin
					for(i = 0; i < 32; i = i + 1)
					begin reg_file[i] <= 32'h0;end
		   end
			else
			begin
				if(wEna)begin reg_file[wAddr] <= wDin;end
			end
	 end
	 
	 always @ (rAddr1 or  rAddr2 or reg_file[rAddr1] or reg_file[rAddr2])
	 begin
			rDout1 = reg_file[rAddr1];
			rDout2 = reg_file[rAddr2];
	 end
endmodule
