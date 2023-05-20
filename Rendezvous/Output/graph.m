load RelPosVel.txt;
data=RelPosVel;
load MonteCarlo.txt;

figure(100);
plot(data(:,1),data(:,2),'-',data(:,1),data(:,3),'-.',data(:,1),data(:,4),'--');
xlabel('time(s)');ylabel('position(m)');
legend('x','y','z');

figure(200);
plot(data(:,1),data(:,5),'-',data(:,1),data(:,6),'-.',data(:,1),data(:,7),'--');
xlabel('time(s)');ylabel('velocity(m/s)')
legend('vx','vy','vz');

figure(300);
plot(data(:,2),data(:,4));
xlabel('x(s)');ylabel('z(m/s)')

figure(400);
%subplot(1,2,1);
plot(MonteCarlo(:,1),MonteCarlo(:,2),'*');
xlabel('x(m)');ylabel('y(m)')

figure(500);
plot(MonteCarlo(:,2),MonteCarlo(:,3),'*');
xlabel('y(m)');ylabel('z(m)')

figure(900);
plot3(MonteCarlo(:,1),MonteCarlo(:,2),MonteCarlo(:,3),'*');
xlabel('x(m)');ylabel('y(m)');zlabel('z(m)')
grid on;

figure(600);
hist(MonteCarlo(:,1),20);
xlabel('x(m)');
figure(700);
hist(MonteCarlo(:,2),20);
xlabel('y(m)');
figure(800);
hist(MonteCarlo(:,3),20);
xlabel('z(m)');

mx=mean(MonteCarlo(:,1))
my=mean(MonteCarlo(:,2))
mz=mean(MonteCarlo(:,3))
sx=std(MonteCarlo(:,1))
sy=std(MonteCarlo(:,2))
sz=std(MonteCarlo(:,3))
mix=min(MonteCarlo(:,1))
miy=min(MonteCarlo(:,2))
miz=min(MonteCarlo(:,3))
maxx=max(MonteCarlo(:,1))
maxy=max(MonteCarlo(:,2))
maxz=max(MonteCarlo(:,3))