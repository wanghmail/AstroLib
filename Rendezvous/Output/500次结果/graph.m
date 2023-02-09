load RelPosVel.txt;
data=RelPosVel;
load MonteCarlo.txt;

figure(100);
subplot(1,2,1);
plot(data(:,1),data(:,2),'-',data(:,1),data(:,3),'-.',data(:,1),data(:,4),'--');
xlabel('time(s)');ylabel('position(m)');
legend('x','y','z');

subplot(1,2,2);
plot(data(:,1),data(:,5),'-',data(:,1),data(:,6),'-.',data(:,1),data(:,7),'--');
xlabel('time(s)');ylabel('velocity(m/s)')
legend('vx','vy','vz');

figure(200);
plot(data(:,2),data(:,4));
xlabel('x(s)');ylabel('z(m/s)')

figure(300);
subplot(1,2,1);
plot(MonteCarlo(:,1),MonteCarlo(:,2),'*');
xlabel('x(m)');ylabel('y(m)')

subplot(1,2,2);
plot(MonteCarlo(:,2),MonteCarlo(:,3),'*');
xlabel('y(m)');ylabel('z(m)')

mx=mean(MonteCarlo(:,1))
my=mean(MonteCarlo(:,2))
mz=mean(MonteCarlo(:,3))
sx=std(MonteCarlo(:,1))
sy=std(MonteCarlo(:,2))
sz=std(MonteCarlo(:,3))