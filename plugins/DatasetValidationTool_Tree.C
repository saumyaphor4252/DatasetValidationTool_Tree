// -*- C++ -*-
//
// Package:    DatasetValidation/DatasetValidationTool
// Class:      DatasetValidationTool_Tree
// 
/**\class DatasetValidationTool_Tree DatasetValidationTool_Tree.cc DatasetValidation/DatasetValidationTool/plugins/DatasetValidationTool_Tree.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Saumya Saumya
//         Created:  Wed, 02 Sep 2020 11:05:57 GMT
//
//


// system include files
#include <memory>
// user include files
#include "CommonTools/TrackerMap/interface/TrackerMap.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "CommonTools/Utils/interface/TFileDirectory.h"
#include "CondFormats/DataRecord/interface/RunSummaryRcd.h"
#include "CondFormats/DataRecord/interface/SiStripCondDataRecords.h"
#include "CondFormats/RunInfo/interface/RunInfo.h"
#include "CondFormats/SiStripObjects/interface/SiStripLatency.h"
#include "DataFormats/BeamSpot/interface/BeamSpot.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/RefTraits.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/GeometrySurface/interface/Plane.h"
#include "DataFormats/GeometryVector/interface/GlobalPoint.h"
#include "DataFormats/GeometryVector/interface/GlobalVector.h"
#include "DataFormats/GeometryVector/interface/LocalPoint.h"
#include "DataFormats/GeometryVector/interface/Phi.h"
#include "DataFormats/GeometryVector/interface/Theta.h"
#include "DataFormats/Math/interface/Point3D.h"
#include "DataFormats/Math/interface/deltaPhi.h"
#include "DataFormats/Provenance/interface/RunLumiEventNumber.h"
#include "DataFormats/SiPixelDetId/interface/PixelSubdetector.h"
#include "DataFormats/SiStripDetId/interface/SiStripDetId.h"
#include "DataFormats/SiStripDetId/interface/StripSubdetector.h"
#include "DataFormats/TrackReco/interface/HitPattern.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackBase.h"
#include "DataFormats/TrackReco/interface/TrackExtraFwd.h"
#include "DataFormats/TrackReco/interface/TrackExtra.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/TrackReco/interface/TrackResiduals.h"
#include "DataFormats/TrackerCommon/interface/TrackerTopology.h"
#include "DataFormats/TrackerRecHit2D/interface/ProjectedSiStripRecHit2D.h"
#include "DataFormats/TrackerRecHit2D/interface/SiPixelRecHit.h"
#include "DataFormats/TrackerRecHit2D/interface/SiStripMatchedRecHit2D.h"
#include "DataFormats/TrackerRecHit2D/interface/SiStripRecHit1D.h"
#include "DataFormats/TrackerRecHit2D/interface/SiStripRecHit2D.h"
#include "DataFormats/TrackingRecHit/interface/TrackingRecHit.h"
#include "DataFormats/TrackingRecHit/interface/TrackingRecHitFwd.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"
#include "FWCore/Framework/src/WorkerMaker.h"
#include "FWCore/MessageLogger/interface/ErrorObj.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ParameterSet/interface/ConfigurationDescriptions.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/ParameterSetDescription.h"
#include "FWCore/ParameterSet/interface/ParameterSetDescriptionFiller.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Utilities/interface/EDGetToken.h"
#include "FWCore/Utilities/interface/ESGetToken.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"
#include "Geometry/Records/interface/TrackerTopologyRcd.h"
#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"
#include "MagneticField/Engine/interface/MagneticField.h"
#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"

#include <TTree.h>
#include <TMath.h>
//
// class declaration
//

// If the analyzer does not use TFileService, please remove
// the template argument to the base class so the class inherits
// from  edm::one::EDAnalyzer<> and also remove the line from
// constructor "usesResource("TFileService");"
// This will improve performance in multithreaded jobs.

class DatasetValidationTool_Tree: public edm::one::EDAnalyzer<edm::one::SharedResources>
{
   public:
      explicit DatasetValidationTool_Tree(const edm::ParameterSet&);
      ~DatasetValidationTool_Tree();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


   private:
      virtual void beginJob() override;
      virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;

      // ----------member data ---------------------------
      edm::EDGetTokenT<reco::TrackCollection> tracksToken_;
 
      int nTracks,nEvents;//,nTracksInEvent,nEventsInRun,nTracksInRun;
      
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
DatasetValidationTool_Tree::DatasetValidationTool_Tree(const edm::ParameterSet& iConfig)
: tracksToken_(consumes<reco::TrackCollection>(iConfig.getParameter<edm::InputTag>("tracks")))//,
{
   //now do what ever initialization is needed
   usesResource("TFileService");
   nTracks=0;nEvents=0;
}


DatasetValidationTool_Tree::~DatasetValidationTool_Tree()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void
DatasetValidationTool_Tree::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{

   using namespace edm;

    edm::Handle<reco::TrackCollection> trackCollection;
    iEvent.getByToken(tracksToken_, trackCollection);
    const reco::TrackCollection tC = *(trackCollection.product());

   for(const auto &track:tC)
   {  
     nTracks++;
     std::cout<<track.pt()<<std::endl;

     auto const &residuals = track.extra()->residuals();

     int h_index = 0;
     int nHits=0;
     for(auto iHit = track.recHitsBegin(); iHit!=track.recHitsEnd(); ++iHit,++h_index)
     {  
           ++nHits;     
          double resX = residuals().residualX(2);
          std::cout<<"Res: "<<resX<<std::endl;
          std::cout<<h_index<<std::endl;
         
     }  //Hits Loop
    std::cout<<"HIts in track"<<nTracks<<": "<<nHits<<std::endl;

   } //Tracks Loop

   nEvents++;

#ifdef THIS_IS_AN_EVENT_EXAMPLE
   Handle<ExampleData> pIn;
   iEvent.getByLabel("example",pIn);
#endif
   
#ifdef THIS_IS_AN_EVENTSETUP_EXAMPLE
   ESHandle<SetupData> pSetup;
   iSetup.get<SetupRecord>().get(pSetup);
#endif
}


// ------------ method called once each job just before starting event loop  ------------
void 
DatasetValidationTool_Tree::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
DatasetValidationTool_Tree::endJob() 
{
   std::cout<<"Events: "<<nEvents<<std::endl;
   std::cout<<"Tracks: "<<nTracks<<std::endl;
 }

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
DatasetValidationTool_Tree::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters

 edm::ParameterSetDescription desc;
 desc.setUnknown();
 descriptions.addDefault(desc);

 // edm::ParameterSetDescription desc;
  //desc.setComment("Create tuple with all variables required to workk with");
 // desc.add<edm::InputTag> ("tracksToken_",edm::InputTag("ALCARECOTkAlCosmicsCTF0T"));
 // descriptions.add("datasetValidationTool",desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(DatasetValidationTool_Tree);
