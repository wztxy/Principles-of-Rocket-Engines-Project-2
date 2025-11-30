%% 1. 读数
M = readmatrix('outdata.csv');          % 文件需在当期文件夹
T = M(:,1);                             % 温度 K
n = M(:,2:9);                           % 8 种组分摩尔数
H = M(:,10);                            % 总焓 J

%% 2. 画图
figure('Color','w');
% 左侧轴：摩尔数
yyaxis left;
p1 = plot(T, n, 'LineWidth',1.2);
xlabel('温度 (K)');
ylabel('平衡摩尔数 (mol)');
grid on;
set(gca,'FontSize',11);

% 右侧轴：总焓（MJ）
yyaxis right;
p2 = plot(T, H/1e6, '-', 'LineWidth',2);
ylabel('总焓 (MJ)');
ylim([min(H/1e6)*1.1, max(H/1e6)*1.1]);

%% 3. 总焓 = –0.933 MJ 水平线 + 交点标注
Htarget = -0.933;
yline(Htarget, 'r--', 'LineWidth',1.5);
hold on;

% 插值找交点
Tfine = linspace(T(1), T(end), 10*numel(T));
Hfine = interp1(T, H/1e6, Tfine, 'linear');
idx   = find(diff(sign(Hfine - Htarget)));   % 符号变化
for k = 1:numel(idx)
    i1 = idx(k); i2 = i1+1;
    t1 = Tfine(i1); t2 = Tfine(i2);
    h1 = Hfine(i1); h2 = Hfine(i2);
    % 线性插值
    T0 = t1 - (h1 - Htarget)*(t2-t1)/(h2-h1);
    plot(T0, Htarget, 'ro', 'MarkerSize',8, 'MarkerFaceColor','r');

    % 文字标注：自动放右侧，若太靠右则放左侧
    ax = gca;
    xn = (T0 - ax.XLim(1))/diff(ax.XLim);
    if xn > 0.7
        dx = -80;  ha = 'right';
    else
        dx = 80;   ha = 'left';
    end
    text(T0+dx, Htarget, sprintf(' %.0f K, %.3f MJ',T0,Htarget), ...
         'Color','r','FontSize',11,'HorizontalAlignment',ha,...
         'VerticalAlignment','middle');
end

%% 4. 图例
legend([p1; p2; yline(Htarget,'r--')], ...
       {'H2O','H2','OH','H','CO2','CO','O2','O','总焓'}, ...
       'Location','best','FontSize',9);
title('平衡组成与总焓随温度变化');