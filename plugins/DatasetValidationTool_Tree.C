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
#include "FWCore/Framework/interface/Run.h"
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
 
      int nTracks,nEvents,nHit1D,nHit2D,nHitsTotal;
      const bool isHit2D(const TrackingRecHit &hit);

      
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
  nTracks=0; nEvents=0; 
}


DatasetValidationTool_Tree::~DatasetValidationTool_Tree()
{
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

   for(reco::TrackCollection::const_iterator track=tC.begin(); track!=tC.end(); track++)
   { 
     nHit2D=0; nHit1D=0; nHitsTotal=0;
     for(auto iHit = track->recHitsBegin(); iHit!=track->recHitsEnd(); ++iHit)
     {   nHitsTotal++;
         if (isHit2D(**iHit)) 
         {
             ++nHit2D;  std::cout<<"2D HIT"<<std::endl;

         }
         else{++nHit1D; std::cout<<"1D Hits"<<std::endl; }
     }  //Hits Loop
     nTracks++;
     std::cout<<"2D Hits"<<nHit2D<<std::endl;
     std::cout<<"1D Hits"<<nHit1D<<std::endl;
     std::cout<<"Total Hits"<<nHitsTotal<<std::endl;
   } //Tracks Loop
   nEvents++;
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

const bool DatasetValidationTool_Tree::isHit2D(const TrackingRecHit &hit)
{
     bool countStereoHitAs2D_ = true;
     // we count SiStrip stereo modules as 2D if selected via countStereoHitAs2D_
     // (since they provide theta information)
     if (!hit.isValid() || (hit.dimension() < 2 && !countStereoHitAs2D_ && !dynamic_cast<const SiStripRecHit1D *>(&hit)))
     {
        return false;  // real RecHit1D - but SiStripRecHit1D depends on countStereoHitAs2D_
     }
     else 
     {
        const DetId detId(hit.geographicalId());
        if (detId.det() == DetId::Tracker)
        {
           if (detId.subdetId() == PixelSubdetector::PixelBarrel || detId.subdetId() == PixelSubdetector::PixelEndcap)
           {
              return true;  // pixel is always 2D
           }
           else
           {        // should be SiStrip now
              const SiStripDetId stripId(detId);
              if(stripId.stereo())  return countStereoHitAs2D_;  // stereo modules
              else if (dynamic_cast<const SiStripRecHit1D *>(&hit) || dynamic_cast<const SiStripRecHit2D *>(&hit)) return false;  // rphi modules hit
              //the following two are not used any more since ages...
              else if (dynamic_cast<const SiStripMatchedRecHit2D *>(&hit)) return true;  // matched is 2D
              else if (dynamic_cast<const ProjectedSiStripRecHit2D *>(&hit))
              {
                 const ProjectedSiStripRecHit2D *pH = static_cast<const ProjectedSiStripRecHit2D *>(&hit);
                 return (countStereoHitAs2D_ && isHit2D(pH->originalHit()));  // depends on original...
              }
              else 
              { // edm::LogError("UnkownType") << "@SUB=DMRChecker::isHit2D"
                //                         << "Tracker hit not in pixel, neither SiStripRecHit[12]D nor "
                //                         << "SiStripMatchedRecHit2D nor ProjectedSiStripRecHit2D.";
                 return false;
              }
            } 
         }
         else 
         {   // not tracker??
            // edm::LogWarning("DetectorMismatch") << "@SUB=DMRChecker::isHit2D"
            //                                 << "Hit not in tracker with 'official' dimension >=2.";
             return true;  // dimension() >= 2 so accept that...
          }
     }// never reached...
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
