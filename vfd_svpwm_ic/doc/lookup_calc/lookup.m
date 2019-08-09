close all;
clear all;
sine_lookup = uint16(1024.*sind(0:1/20:60));
output = dec2hex(sine_lookup,4);

fileID = fopen('sine.txt','w');
x = 1;

for i = 1:1:150
    fprintf(fileID, '    .dw     ');
    for j = 1:1:7
        fprintf(fileID, '0x');
        fprintf(fileID, output(x, 1:4));
        fprintf(fileID, ', ');
        disp(x);
        x = x + 1;
    end
    
    fprintf(fileID, '0x');
    fprintf(fileID, output(x, 1:4));
    fprintf(fileID, ' \n');
    x = x + 1;
    disp(x);
end
