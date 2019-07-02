## scripts directory


<!-- Put your Marlin steering files here and explain to which processor they refer (if the file name is not obvious) ! -->

# How to run

1. Create following symbolic links.
* data_idr_500: to IDR dst-merged sample dir
* isolated_electron_weights: to weight file dir for electron in IsolatedLeptonTagging, MarlinReco
* isolated_muon_weights_woYoke: to weight file dir for muon w/o Yoke in IsolatedLeptonTagging, MarlinReco
* lcfiweights: to same name dir in LCFIPlusConfig, ILDConfig
* vtxprob: to same name dir in LCFIPlusConfig, ILDConfig

2. run goSteve
* ./goSteve test: test mode
* ./goSteve all: for all the target
* ./goSteve sig: for signal only
* ./goSteve bkg: for backgrond only