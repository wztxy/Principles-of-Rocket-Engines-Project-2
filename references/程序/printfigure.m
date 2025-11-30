%% 1. 读文件
raw = fileread('outdata.csv');

%% 2. 清洗
raw = regexprep(raw,',\s*$','');           % 去掉行尾逗号
raw = strjoin(strsplit(raw,{'\n','\r'}));  % 合并行
nums = sscanf(raw,'%f,');                  % 按逗号解析

%% 3.  reshape
nSpec = 8;
data  = reshape(nums, nSpec, []).';        % 每列一条曲线

%% 4. 画图
figure('Color','w','Position',[400 200 900 400]);
loglog(data,'LineWidth',1.3);
 grid on
xlabel('迭代次数'); ylabel('组分摩尔数 / mol');
title('燃烧反应各组分摩尔数迭代历程');
legend({'H2O','H2','OH','H','CO2','CO','O2','O'},...
       'Location','eastoutside','Orientation','vertical');

%% 5. 可选：导出高清图
% print(gcf,'species_iter.png','-dpng','-r300')