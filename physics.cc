#include "physics.hh"

MyPhysicsList::MyPhysicsList()
:G4VModularPhysicsList()
{
  G4int verb = 2;
  SetVerboseLevel(verb);

  // Mandatory for G4NuclideTable
  // Half-life threshold must be set small or many short-lived isomers 
  // will not be assigned life times (default to 0) 
  G4NuclideTable::GetInstance()->SetThresholdOfHalfLife(0.1*picosecond);
  G4NuclideTable::GetInstance()->SetLevelTolerance(1.0*eV);
          
  // EM physics
  RegisterPhysics(new G4EmStandardPhysics());
  G4EmParameters* param = G4EmParameters::Instance();
  param->SetAugerCascade(true);
  param->SetStepFunction(1., 1*CLHEP::mm);
  param->SetStepFunctionMuHad(1., 1*CLHEP::mm);

  // Optical Physics
  auto opticalPhysics = new G4OpticalPhysics();
  auto opticalParams= G4OpticalParameters::Instance();
  opticalParams->SetWLSTimeProfile("delta");
  opticalParams->SetScintTrackSecondariesFirst(true);
  opticalParams->SetCerenkovMaxPhotonsPerStep(1); // FIXME: Fake value for fast check. Use 100 for production.
  opticalParams->SetCerenkovMaxBetaChange(10.0);
  opticalParams->SetCerenkovTrackSecondariesFirst(true);
  RegisterPhysics(opticalPhysics);

  // Decay
  RegisterPhysics(new G4DecayPhysics());
            
  // Hadron Elastic scattering
  RegisterPhysics( new G4HadronElasticPhysics(verb) );
  
  // Hadron Inelastic physics
  RegisterPhysics( new G4HadronPhysicsFTFP_BERT(verb));
  
  // Ion Elastic scattering
  RegisterPhysics( new G4IonElasticPhysics(verb));
      
  // Ion Inelastic physics
  RegisterPhysics( new G4IonPhysics(verb));
    
  // Gamma-Nuclear Physics
  G4EmExtraPhysics* gnuc = new G4EmExtraPhysics(verb);
  gnuc->ElectroNuclear(false);
  gnuc->MuonNuclear(false);
  RegisterPhysics(gnuc);
}

MyPhysicsList::~MyPhysicsList()
{ }

void MyPhysicsList::ConstructParticle()
{
  G4BosonConstructor  pBosonConstructor;
  pBosonConstructor.ConstructParticle();

  G4LeptonConstructor pLeptonConstructor;
  pLeptonConstructor.ConstructParticle();

  G4MesonConstructor pMesonConstructor;
  pMesonConstructor.ConstructParticle();

  G4BaryonConstructor pBaryonConstructor;
  pBaryonConstructor.ConstructParticle();

  G4IonConstructor pIonConstructor;
  pIonConstructor.ConstructParticle();

  G4ShortLivedConstructor pShortLivedConstructor;
  pShortLivedConstructor.ConstructParticle();  
}

void MyPhysicsList::SetCuts()
{
  SetCutValue(0*mm, "proton");
  SetCutValue(10*km, "e-");
  SetCutValue(10*km, "e+");
  SetCutValue(10*km, "gamma");      
}

