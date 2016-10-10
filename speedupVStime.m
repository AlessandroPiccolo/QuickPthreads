% speedup
clear all; clc; close all;

% #threads
threads = [1 4 9 16 25 36];

% Experimental data

% size 90; 1 proc 4 proc 9 proc 25 proc 36 proc
pear = [0.0035420 0.0031470 0.003303 0.003179 1.151207 1.151207
           0.0035420 0.0031000 0.003503 0.003180 1.391103 1.151207
           0.0035490 0.0031969 0.002825 0.003161 1.180641 1.151207];
% size 360: 1 proc 4 proc 9 proc 16 proc 25 proc 36 proc
dac = [0.223261 0.067356 0.068778 0.060817 0.041221 0.040320
            0.179508 0.071412 0.419592 0.062333 0.131206 0.191579
            0.186088 0.069561 0.121910 0.061397 0.102034 0.041240];

time = [mean(pear);
        mean(dac)];

d = {'Pear' 'DAC'};
nDifferentSizes = length(d);
speedup = zeros(nDifferentSizes, length(threads));

% Calculating speedup, S = Tinit/Tnew
for i = 1:nDifferentSizes
    speedup(i,:) = time(i,1)./time(i,:);
end

for j = 1:nDifferentSizes
    figure(j)
    [ax,p1,p2] = plotyy(threads,time(j,:),threads,speedup(j,:),'semilogy','plot', 'LineWidth',2);
    hold(ax(1), 'on')
    hold(ax(2), 'on')
    plot(ax(1),threads,time(j,:),'sb')
    plot(ax(2),threads,speedup(j,:),'sr')
    hold(ax(1), 'off')
    hold(ax(2), 'off')
    set(ax,'FontSize',16, 'FontWeight', 'bold');
    title(['Time cost and speedup for ' d{j}]);
    ylabel(ax(2),'Speedup') % label left y-axis
    ylabel(ax(1),'Time cost [s]') % label right y-axis
    xlabel(ax(1),'Threads') % label x-axis
    grid(ax(1),'on')
end
