int External()
{
    std::string path{"o2sim_Kine.root"};

    TFile file(path.c_str(), "READ");
    if (file.IsZombie())
    {
        std::cerr << "Cannot open ROOT file " << path << "\n";
        return 1;
    }

    auto tree = (TTree *)file.Get("o2sim");
    if (!tree)
    {
        std::cerr << "Cannot find tree o2sim in file " << path << "\n";
        return 1;
    }
    std::vector<o2::MCTrack> *tracks{};
    tree->SetBranchAddress("MCTrack", &tracks);

    auto nEvents = tree->GetEntries();
    auto nInjected = tree->Scan("MCTrack.GetPdgCode()", "MCTrack.GetPdgCode() == -1000020030"); /// don't check matter, too many secondaries
    nInjected += tree->Scan("MCTrack.GetPdgCode()", "MCTrack.GetPdgCode() == -1000010030"); /// don't check matter, too many secondaries
    nInjected += tree->Scan("MCTrack.GetPdgCode()", "TMath::Abs(MCTrack.GetPdgCode()) == 1010010030"); /// don't check matter, too many secondaries
    if (nInjected == 0)
    {
        std::cerr << "Unexpected ratio of events to injected nuclei\n";
        return 1;
    }
    return 0;
}
