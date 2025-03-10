fid = fopen('voo_sim.txt', 'r');  
dados = textscan(fid, '%f %f %f %f %f', 'Delimiter', ' ', 'HeaderLines', 1); 
fclose(fid);

x = dados{1};
y = dados{2};

plot(x, y, 'ro'); % 'ro' para pontos vermelhos
xlabel('t(s)');
ylabel('v(m/s)');
title('Gráfico da trajetória');