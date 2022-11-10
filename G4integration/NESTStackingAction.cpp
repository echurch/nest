#include "NESTStackingAction.hh"
#include "G4OpticalPhoton.hh"
#include "NESTProc.hh"
#include <iostream>

NESTStackingAction::NESTStackingAction()  {cnt=0;}
NESTStackingAction::~NESTStackingAction() {}

NESTStackingAction* NESTStackingAction::theStackingAction = 0;
G4StackManager* NESTStackingAction::savedManager = 0;

G4ClassificationOfNewTrack NESTStackingAction::ClassifyNewTrack(
    const G4Track* track) {
  savedManager = stackManager;

  //  std::cout << "NESTStackingAction::ClassifyNewTrack!!!!!" << std::endl;

  if (track->GetDefinition() == G4OpticalPhoton::OpticalPhotonDefinition()) {

    // Kill a lot of the S1 photons Just so I can see the underlying MeV e-. EC, 21-June-2022.
    const G4ThreeVector pos(track->GetPosition()); //GetVertexPosition() returns all 0's.
    const G4double time(track->GetGlobalTime());
    const G4ParticleDefinition* particle = track->GetParticleDefinition();
    G4int pID       = particle->GetPDGEncoding();
    if (abs(pID)==22 && pos[1]/mm<540.0 && G4UniformRand()<0.999) // gasBoxH=60cm with a 0.5 cm LEM gap
      {
	//	std::cout << "SteppingAction::UserSteppingAction() Killing S1 optical photon at " << pos[0] <<"," <<pos[1]<<","<<pos[2] << ",," << time << std::endl;
	//	return fKill;
      }

        return fWaiting;  

  }

  // This conditional throws an error, so let's just assume if we're here it's a thermale.
  //  if (track->GetDefinition() == NEST::NESTThermalElectron::ThermalElectronDefinition())
  //    {
  cnt++;
  //  if (!(cnt%100)) G4cout << "NESTStackingAction::ClassifyNewTrack thermale count is " << cnt << G4endl;
      //    }


  return fUrgent;
}

bool NESTStackingAction::isUrgentEmpty() {
  if (!savedManager) {
    std::cerr
        << "savedManager not set by NESTStackingAction::ClassifyNewTrack(). "
           "Did you set up NESTStackingAction as your stacking action? Did you "
           "override ClassifyNewTrack and forget to set savedManager?"
        << std::endl;
  }
  return savedManager->GetNUrgentTrack() == 0;
}
void NESTStackingAction::NewStage() {}
