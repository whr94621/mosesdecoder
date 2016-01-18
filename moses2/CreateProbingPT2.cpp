#include <string>
#include <boost/program_options.hpp>
#include "util/usage.hh"
#include "legacy/ProbingPT/storing.hh"

using namespace std;

int main(int argc, char* argv[])
{
	string inPath, outPath; 
	int num_scores = 4;
	int num_lex_scores = 0;
	bool log_prob = false;
	int max_cache_size = 50000;

  namespace po = boost::program_options;
  po::options_description desc("Options");
  desc.add_options()
  ("help", "Print help messages")
  ("input-pt", po::value<string>()->required(), "Text pt")
  ("output-dir", po::value<string>()->required(), "Directory when binary files will be written")
  ("num-scores", po::value<int>()->default_value(num_scores), "Number of pt scores")
  ("num-lex-scores", po::value<int>()->default_value(num_lex_scores), "Number of lexicalized reordering scores")
  ("log-prob", "log (and floor) probabilities before storing")
  ("max-cache-size", po::value<int>()->default_value(max_cache_size), "Maximum number of high-count source lines to write to cache file. 0=no cache, negative=no limit")

	;

  po::variables_map vm;
  try {
    po::store(po::parse_command_line(argc, argv, desc),
              vm); // can throw

    /** --help option
     */
    if ( vm.count("help")) {
      std::cout << desc << std::endl;
      return EXIT_SUCCESS;
    }

    po::notify(vm); // throws on error, so do after help in case
    // there are any problems
  } catch(po::error& e) {
    std::cerr << "ERROR: " << e.what() << std::endl << std::endl;
    std::cerr << desc << std::endl;
    return EXIT_FAILURE;
  }

  if (vm.count("input-pt")) inPath = vm["input-pt"].as<string>();
  if (vm.count("output-dir")) outPath = vm["output-dir"].as<string>();
  if (vm.count("num-scores")) num_scores = vm["num-scores"].as<int>();
  if (vm.count("num-lex-scores")) num_lex_scores = vm["num-lex-scores"].as<int>();
  if (vm.count("max-cache-size")) max_cache_size = vm["max-cache-size"].as<int>();
  if (vm.count("log-prob")) log_prob = true;


  Moses2::createProbingPT(inPath, outPath, num_scores, num_lex_scores, log_prob, max_cache_size);

  util::PrintUsage(std::cout);
  return 0;
}

