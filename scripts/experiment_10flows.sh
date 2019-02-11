DIR=/home/vamsi/src/detnet/scripts/results
cd $NS3
FILE=$NS3/scratch/detnet_2.cc
LR=1000
#U=$(( $LR-100 ))
FDN=100;
rm $DIR/exp.dat
rm $DIR/exp_10flows_tx.dat
rm $DIR/exp_10flows_rx.dat
while [[ $FDN -le $LR ]];do
	#FOTH=$(( $LR-$FDN ))
	echo -e "Rate Detnet = $FDN\n"
	APP=0;
	while [[ $APP -le 9 ]];do
	sed -i -e 's#onoff.SetConstantRate (DataRate (".*Kbps")); //app'$APP'#onoff.SetConstantRate (DataRate ("'$FDN'Kbps")); //app'$APP'#g' $FILE
	APP=$(( $APP+1 ))
	done
	./waf --run detnet_2 2> /dev/null > $DIR/flow_stats.dat
	cat $DIR/flow_stats.dat | grep ThroughputRx >> $DIR/exp_10flows_rx.dat
	cat $DIR/flow_stats.dat | grep ThroughputTx >> $DIR/exp_10flows_tx.dat
	#python $DIR/../flowmon-parse-results.py $NS3/BwResv-flowMonitor.xml > $DIR/flow_stats.dat
	#FLOW_1_TX=$(cat $DIR/flow_stats.dat | grep "10.1.4.1" -a1 | tail -n1 | awk '{print $3}')
	#FLOW_1_RX=$(cat $DIR/flow_stats.dat | grep "10.1.4.1" -a2 | tail -n1 | awk '{print $3}')
	#FLOW_2_TX=$(cat $DIR/flow_stats.dat | grep "10.1.3.1" -a1 | tail -n1 | awk '{print $3}')
	#FLOW_2_RX=$(cat $DIR/flow_stats.dat | grep "10.1.3.1" -a2 | tail -n1 | awk '{print $3}')
	#echo -e "$FLOW_1_TX\t$FLOW_1_RX\t$FLOW_2_TX\t$FLOW_2_RX" >> $DIR/exp.dat
	FDN=$(( $FDN+100 ))
done
