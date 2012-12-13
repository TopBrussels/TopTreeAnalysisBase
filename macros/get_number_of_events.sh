for i in `seq $1 $7 $2`;do
for j in `seq $3 $7 $4`;do





if [ -f ~/files/MG/7TeV/NewRound/TopTree/RND_"$5"_to_"$6"/m0_"$i"_m12_"$j"/TopTree_m0_"$i"_m12_"$j".root ]; then

rm ~/files/MG/7TeV/NewRound/TopTree/RND_"$5"_to_"$6"/m0_"$i"_m12_"$j"/Number_of_events_m0_"$i"_m12_"$j"_RND_"$5"_to_"$6"

rm text*;
rm Number_*;

if [ -f /user/alkaloge/Workspace/newround/Delphes_V_1.8iihe/Delphes_m0_"$i"_m12_"$j"_RND_"$5"_to_"$6"_run.log ]; then

cp /user/alkaloge/Workspace/newround/Delphes_V_1.8iihe/Delphes_m0_"$i"_m12_"$j"_RND_"$5"_to_"$6"_run.log text.log

awk 'BEGIN{FS=" Time report for "}/ / {print $2}' text.log > text1
awk 'BEGIN{FS=" "}/ / {print $1}' text1 > Number_of_events_m0_"$i"_m12_"$j"_RND_"$5"_to_"$6"

cp Number_of_events_m0_"$i"_m12_"$j"_RND_"$5"_to_"$6" ~/files/MG/7TeV/NewRound/TopTree/RND_"$5"_to_"$6"/m0_"$i"_m12_"$j"/.


fi

fi


done
done


