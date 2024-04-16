clear;
% 创建一个 VideoWriter 对象
writerObj = VideoWriter('my_animation.mp4', 'MPEG-4');
writerObj.FrameRate = 10; % 设置帧率

% 打开 VideoWriter 对象
open(writerObj);

% 创建一个图形窗口


load RelPosVel.txt;
data=RelPosVel;


figure(1);
x=plot(data(1,2),-data(1,4),'o');
hold on
xx=plot(data(1,2),-data(1,4));
a=plot(0,0,'o');
b=plot(data(1,5),-data(1,7),'o');
c=plot(data(1,8),-data(1,10),'o');
xlabel('v-bar(m)');ylabel('r-bar(m)');

axis equal
axis([-1e5,3e5,-5e4,5e4])

step=50;
s=size(data)/step;

for j=1:s(1)
    set(xx,'xdata',data(1:10:j*step,2),'ydata',-data(1:10:j*step,4));
    set(x,'xdata',data(j*step,2),'ydata',-data(j*step,4));
    set(b,'xdata',data(j*step,5),'ydata',-data(j*step,7));
    set(c,'xdata',data(j*step,8),'ydata',-data(j*step,10));
    drawnow
    % 将当前图形帧写入 VideoWriter 对象
    writeVideo(writerObj, getframe(gcf));
end

legend([a,b,c],{'目标1','目标2','目标3'});
% 关闭 VideoWriter 对象
close(writerObj);