# DataVolumeProjection
Empirically project the amount of disk space that an ongoing LHC run will consume on disk.



First, I need to get all of the file sizes (per PD) from DAS. 
This is done with this script which does both AOD and RAW: 

/afs/cern.ch/work/k/kamin/public/Run2016/EventCounting/FetchRunInfo.sh

eg. 
````
echo "PADTrack1"
python ./das_client.py --query "summary  dataset=/PADTrack1/PARun2016C-v1/RAW"
echo "PADTrack2"
python ./das_client.py --query "summary  dataset=/PADTrack2/PARun2016C-v1/RAW"
echo "PADoubleMuon"
python ./das_client.py --query "summary  dataset=/PADoubleMuon/PARun2016C-v1/RAW"
````
... and so on, which gets the PD sizes and number of events in each PD. In principle, 
this only needs to be done every so often.
https://cmsweb.cern.ch/das/request?view=list&limit=50&instance=prod%2Fglobal&input=dataset%3D%2FPADoubleMuon%2FPARun2016C*%2FAOD




Unfortunately, this then needs to have all the blank lines removed before I can 
rn 
GetSizes.py
over it to get the event sizes. 

To this point, I've just been running the script and then copying the info into 
the main drawing script: 
~/Dropbox/work/code/Run2016/DataVolumeProjection/PlotEventsVsTimeFill.C




Then I run Emilien's script and bunch of time from this bash script: 
/afs/cern.ch/work/k/kamin/public/Run2016/EventCounting/MyCommands_Fill_ALL.sh
````
echo "NEWJSONRUN  5519 "
./recorded_by_hltpath_fromDQM  --type Datasets  --basedir /afs/cern.ch/user/e/echapon/workspace/public/RunPrep2016/HLT_runbyrun/DQMfiles/  --minrun 285479  --maxrun 285483
```` and so on... 
every day, adding the newest fill.
btw, the new '--extrapolate' option extrapolates for missing lumis in DQM.  it's ~10-15% !! 




Then run PlotEventsVsTimeFill.C\( date \) with the newest date and you'll get the plots ! 

See RunAll.sh for examples of how to run this plotting script.  Choose whether you want RAW, AOD or RAW+AOD (2nd argument=1,2,3).  You can also choose the output in base2 or base10 (3rd argument, for the data GB unit).

