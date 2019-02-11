DIR=/home/vamsi/src/detnet/scripts/results
cd $NS3
FILE=$NS3/scratch/detnet_2.cc
LR=20000
U=$(( $LR-1000 ))
FDN=1000;
rm $DIR/exp.dat
while [[ $FDN -le $U ]];do
	FOTH=$(( $LR-$FDN ))
	echo -e "Rate Detnet = $FDN\tRate Other = $FOTH\n"
	sed -i -e 's/onoff.SetConstantRate (DataRate (".*kbps"));/onoff.SetConstantRate (DataRate ("'$FDN'kbps"));/g' $FILE #Onoff is detnet flow
	sed -i -e 's/onoff1.SetConstantRate (DataRate (".*kbps"));/onoff1.SetConstantRate (DataRate ("'$FOTH'kbps"));/g' $FILE #Onoff1 is other flow
	./waf --run detnet_2
	python $DIR/../flowmon-parse-results.py $NS3/BwResv-flowMonitor.xml > $DIR/flow_stats.dat
	FLOW_1_TX=$(cat $DIR/flow_stats.dat | grep "10.1.4.1" -a1 | tail -n1 | awk '{print $3}')
	FLOW_1_RX=$(cat $DIR/flow_stats.dat | grep "10.1.4.1" -a2 | tail -n1 | awk '{print $3}')
	FLOW_2_TX=$(cat $DIR/flow_stats.dat | grep "10.1.3.1" -a1 | tail -n1 | awk '{print $3}')
	FLOW_2_RX=$(cat $DIR/flow_stats.dat | grep "10.1.3.1" -a2 | tail -n1 | awk '{print $3}')
	echo -e "$FLOW_1_TX\t$FLOW_1_RX\t$FLOW_2_TX\t$FLOW_2_RX" >> $DIR/exp.dat
	FDN=$(( $FDN+1000 ))
done
