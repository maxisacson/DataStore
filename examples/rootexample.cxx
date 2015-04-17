#include "DataStore.h"
#include <TFile.h>
#include <TTree.h>
#include <TH1.h>
#include <vector>

int main() {
  TFile* f = TFile::Open("tree.root");
  TFile* out = TFile::Open("out.root", "RECREATE");
  TH1D * h = new TH1D("gaus", "gaussian", 50, -5, 5);
  TH1D * m = new TH1D("gaus_m", "gaussian(80, 2)", 50, 70, 90);
  TTree * tree = (TTree*)f->Get("tree");

  /**
   * add desired variables to the store
   */
  ds::DataStore store;
  store.add<float>("gaus");
  store.add<std::vector<float> >("m");

  /**
   * get references to the objects in the store. Note the use of the ptr 'addr_v'
   * to make root accept the address. When pointing branches to vectors root needs
   * the address to the pointer to the vector.
   */
  float& x = store.retrieve<float>("gaus");
  std::vector<float>& v = store.retrieve<std::vector<float> >("m");
  std::vector<float>* addr_v = &v;

  /**
   * set the branch addresses
   */
  tree->SetBranchAddress("gauss", &x);
  tree->SetBranchAddress("m", &addr_v);

  /**
   * get new references to the objects, just to illustrate that the objects are
   * actually updated in the store.
   */
  float& y = store.retrieve<float>("gaus");
  std::vector<float>& v_m = store.retrieve<std::vector<float> >("m");

  /**
   * loop over the entries in the tree
   */
  long n = tree->GetEntries();
  for (int i = 0; i < n; i++) {
    /**
     * get entry #i
     */
    tree->GetEntry(i);

    /**
     * fill our histograms
     */
    h->Fill(y);
    for (unsigned int j = 0; j < v_m.size(); j++) {
      m->Fill(v_m[j]);
    }
  }

  /**
   * write and close
   */
  out->Write();
  out->Close();
  f->Close();

  return 0;
}
