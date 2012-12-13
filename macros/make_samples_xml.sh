for i in `seq $1 $7 $2`;do
for j in `seq $3 $7 $4`;do




if [ ! -f ~/files/MG/7TeV/NewRound/TopTree/RND_"$5"_to_"$6"/m0_"$i"_m12_"$j"/TopTree_m0_"$i"_m12_"$j".root ]; then

echo dataset for m0 = $i m0= $j 

fi


if [ -f ~/files/MG/7TeV/NewRound/TopTree/RND_"$5"_to_"$6"/m0_"$i"_m12_"$j"/TopTree_m0_"$i"_m12_"$j".root ]; then

#echo

#ls ~/files/MG/7TeV/NewRound/TopTree/RND_"$5"_to_"$6"/m0_"$i"_m12_"$j"/TopTree_m0_"$i"_m12_"$j".root





awk 'BEGIN{FS="#"}// {print $1}' ~/files/MG/7TeV/NewRound/TopTree/RND_"$5"_to_"$6"/m0_"$i"_m12_"$j"/CrossX > cx

awk 'BEGIN{FS="#"}// {print $1}' ~/files/MG/7TeV/NewRound/TopTree/RND_"$5"_to_"$6"/m0_"$i"_m12_"$j"/Number_of_events_m0_"$i"_m12_"$j"_RND_"$5"_to_"$6" > number

cat number | while read nevts; do
cat cx | while read crossx; do





sed  '$a<d name="SUSY" title="m0_'"$i"'_m12_'"$j"'" add="1" color="4"  ls="1" lw="1" normf="1"   xsection="'"$crossx"'"   NofPSEvts="'"$nevts"'"  filenames="/user/alkaloge/files/MG/7TeV/NewRound/TopTree/RND_'"$5"'_to_'"$6"'/m0_'"$i"'_m12_'"$j"'/TopTree_m0_'"$i"'_m12_'"$j"'.root"/> '  myfile >file_"$i"_"$j" 

done
done

#awk ' { print$0"Add Text to end_ "$i"" } ' myfile > myfile1

#sed <d name="SUSY" title="SUSY" add="1" color="4"  ls="1" lw="1" normf="1" PreselEff="0.5818" NofPSEvts="50000"   xsection="170"    filenames="/user/alkaloge/files/MG/7TeV/NewRound/TopTree/RND_1_to_15000/m0_100_m12_100/TopTree_m0_100_m12_100.root"/>


fi


#less file_* > total

#sed '/^ *$/d' file > total_SUSY_Samples


done
done

less file_* > total

sed '/^ *$/d' total > total_SUSY_Samples



