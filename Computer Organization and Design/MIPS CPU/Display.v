`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    21:07:17 05/31/2018 
// Design Name: 
// Module Name:    Display 
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
module Display(
	input clk,
	input display,
	input [7:0] sel_addr,
	output reg [7:0] real_addr
    );
	 reg [31:0] cnt;
	 reg [7:0] addr;
	 parameter intvl = 200000000;
	 
	 always @ (posedge clk)
	 begin
			
			if(display == 1'b1)
			begin
				if(cnt == intvl)
				begin
					cnt <= 32'h0;
					addr <= addr + 8'h1;
					real_addr <= addr + 8'h1;
				end
				else
				begin
					cnt <= cnt + 32'h1;
					real_addr <= addr;
				end
			end
			else
			begin
				real_addr <= sel_addr;
				addr <= 8'h0;
			end
	 end
endmodule
