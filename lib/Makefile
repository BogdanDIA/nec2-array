parse_sweep: parse_sweep.cc
	g++ -o parse_sweep parse_sweep.cc

generate_sweep: generate_sweep.cc
	g++ -o generate_sweep generate_sweep.cc

parse_mutual: parse_mutual.cc
	g++ -o parse_mutual parse_mutual.cc

generate_mutual: generate_mutual.cc
	g++ -o generate_mutual generate_mutual.cc

parse_one: parse_one.cc
	g++ -o parse_one parse_one.cc

generate_one: generate_one.cc
	g++ -o generate_one generate_one.cc

all : parse_sweep generate_sweep parse_one generate_one generate_mutual parse_mutual

clean:
	rm -rf generate_one
	rm -rf parse_one
	rm -rf parse_sweep
	rm -rf generate_sweep
	rm -rf parse_mutual
	rm -rf generate_mutual
