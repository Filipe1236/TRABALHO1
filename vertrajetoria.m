fid = fopen('voo_sim.txt', 'r');  
dados = textscan(fid, '%f %f %f %f %f', 'Delimiter', ' ', 'HeaderLines', 1); 
fclose(fid);

x = dados{4};
y = dados{5};

plot(x, y, 'ro'); % 'ro' para pontos vermelhos
xlabel('x(m)');
ylabel('h(m)');
title('Gráfico da trajetória');