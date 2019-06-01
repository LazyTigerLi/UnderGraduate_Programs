`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2019/06/01 01:54:17
// Design Name: 
// Module Name: bht
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


module bht #(
    parameter LOW_ADDR_LEN = 5    //低位地址长度，低位地址用于在cache内寻址，也决定了cache的大小
)(
    input clk,
    input rst,
    input wire [31:0] pc,
    input wire isBranch,    //指示pc_update是不是分支指令
    input wire [31:0] pc_update,
    input wire takenE,
    output reg takenF
);
localparam PC_CNT = 1 << LOW_ADDR_LEN;
localparam UNUSED_ADDR_LEN = 30 - LOW_ADDR_LEN;


reg [1 : 0] status [PC_CNT];

reg [LOW_ADDR_LEN - 1:0] low_addr;
reg [LOW_ADDR_LEN - 1:0] index_update;
assign low_addr = pc[1 + LOW_ADDR_LEN:2];
assign index_update = pc_update[1 + LOW_ADDR_LEN:2];

always @ (pc)
begin
    if(status[low_addr] == 2'b00 || status[low_addr] == 2'b01)takenF = 1'b0;
    else takenF = 1'b1;
end

always @ (negedge clk or posedge rst)
begin
    if(rst)
    begin
        for(integer i = 0; i < PC_CNT; i++)
        begin
            status[i] = 2'b00;
        end
    end
    else begin
        if(isBranch)
        begin
            case(status[index_update])
                2'b00:begin if(takenE)status[index_update] = 2'b01;
                            else status[index_update] = 2'b00; end
                2'b01:begin if(takenE)status[index_update] = 2'b11;
                            else status[index_update] = 2'b00; end
                2'b10:begin if(takenE)status[index_update] = 2'b11;
                            else status[index_update] = 2'b00; end
                2'b11:begin if(takenE)status[index_update] = 2'b11;
                            else status[index_update] = 2'b10; end
            endcase
        end
    end
end
endmodule