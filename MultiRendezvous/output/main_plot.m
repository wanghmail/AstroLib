clear;
load RelPosVel.txt;
data=RelPosVel;


figure(1);
plot(data(:,2),-data(:,4));
xlabel('v-bar(m)');ylabel('r-bar(m)');
axis equal
axis([-1e5,2.5e5,-4e4,2e4])

hold on
aim1=find(RelPosVel(:,11)==1);
plot(data(aim1,2),-data(aim1,4),'LineWidth',3);
aim2=find(RelPosVel(:,11)==2);
plot(data(aim2,2),-data(aim2,4),'LineWidth',3);
aim3=find(RelPosVel(:,11)==3);
plot(data(aim3,2),-data(aim3,4),'LineWidth',3);

print('MulRend.png','-dpng','-r1000')