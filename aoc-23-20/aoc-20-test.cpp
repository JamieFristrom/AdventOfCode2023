#include <gtest/gtest.h>

#include <map>
#include <queue>
#include <regex>

#include "pex.h"

using namespace pex;

enum class Pulse {
    Low,
    High
};

class Module;
class SandSystem;

using LivePulse = std::tuple<std::string, Pulse, std::string>;

using SandSystemUPtr = std::unique_ptr<SandSystem>;

int hack = 0;

class SandSystem {
public:
    void sendPulse(const std::string& source, const Pulse pulse, const std::string& destination) {
        //std::cout << source << " -" << (pulse == Pulse::Low ? "low" : "high") << "-> " << destination << std::endl;
        if (destination == "rx") {
            if (pulse == Pulse::Low) {
                hack++;
            }
        }
        mLivePulses.push(LivePulse{ source, pulse, destination });
        if (pulse == Pulse::Low) {
            ++lowCount;
        }
        else {
            ++highCount;
        }
    }

    // system, low, high
    static std::tuple<SandSystemUPtr, int, int> process(SandSystemUPtr sSystem, const int64_t tick);

    static SandSystem parseInput(const std::string& input);

private:
    std::map<std::string, std::unique_ptr<Module>> mModules;
    std::queue<LivePulse> mLivePulses;
    int lowCount = 0;
    int highCount = 0;
};



// am I really using inheritance in AOC? This might be a first.
// I am also playing around with the idea of borrowing mutable unique_ptrs and returning them
// to be "rusty." shared_ptr sure is more ergonomic...though less safe...
class Module {
    public:
    std::string name;  // duplication of truth, wah
    virtual SandSystemUPtr receivePulse(SandSystemUPtr sSystem, const std::string&, const Pulse pulse, const int tick) {
        return sSystem;
    }
    
    virtual void initialize(const std::string& src) {}

    SandSystemUPtr _sendPulse(SandSystemUPtr sSystem, const std::string& source, const Pulse pulse) {
        for (auto& destination : destinations) {
            sSystem->sendPulse(source, pulse, destination);
        }
        return sSystem;
    }

    std::vector<std::string> destinations;
};

// Flip-flop modules (prefix %) are either on or off; they are initially off. 
// If a flip-flop module receives a high pulse, it is ignored and nothing happens. 
// However, if a flip-flop module receives a low pulse, it flips between on and off. 
// If it was off, it turns on and sends a high pulse. 
// If it was on, it turns off and sends a low pulse.
class FlipFlopModule : public Module {
    bool on=false;
    public:
    virtual SandSystemUPtr receivePulse(SandSystemUPtr sSystem, const std::string&, const Pulse pulse, const int tick) {
        if (pulse == Pulse::High) {
            // ignore
        }
        else {
            on = !on;
            sSystem = _sendPulse(std::move(sSystem), name, on ? Pulse::High : Pulse::Low);
        }
        return sSystem;
    }
};

// Conjunction modules(prefix&) remember the type of the most recent pulse received 
// from each of their connected input modules; 
// they initially default to remembering a low pulse for each input.
class ConjunctionModule : public Module {
    public:
        virtual void initialize(const std::string& source) {
            mLastInputs[source] = Pulse::Low;
        }

        virtual SandSystemUPtr receivePulse(SandSystemUPtr sSystem, const std::string& source, const Pulse pulse, const int tick) {
            // When a pulse is received, the conjunction module first updates its memory for that input.
            // Then, if it remembers high pulses for all inputs, it sends a low pulse; 
            // otherwise, it sends a high pulse.
            mLastInputs[source] = pulse;
            if (name == "sq") {
                if (pulse == Pulse::High) {// || mLastInputs[source] == Pulse::High) {  // second thing is "window closing"
                    if (true) { //pCountIf(mLastInputs, [](let p) { return p.second == Pulse::High; }) >= 2) {
                        std::cout << "Tick " << tick << ": " << name << " received " << ((pulse == Pulse::Low) ? "low" : "high") << " pulse from " << source << ". ";
                        for (let& inputPair : mLastInputs) {
                            std::cout << inputPair.first << ":" << ((inputPair.second == Pulse::Low) ? "low" : "high") << " ";
                        }

                        std::cout << std::endl;
                    }
                }
            }

            let outPulse = pAllOf(mLastInputs, [](let pulse) {return pulse.second == Pulse::High; }) ?
                Pulse::Low :
                Pulse::High;
            
            sSystem = _sendPulse(std::move(sSystem), name, outPulse);
            return sSystem;
        }

        std::map<std::string, Pulse> mLastInputs;
};

class BroadcastModule : public Module {
    public:
        virtual SandSystemUPtr receivePulse(SandSystemUPtr sSystem, const std::string&, const Pulse pulse, const int tick) {
            return _sendPulse(std::move(sSystem), name, pulse);
        }
};

std::unique_ptr<Module> moduleFactory(const std::string& type, const std::string& name) {
    auto result =  
        type.empty()?static_cast<std::unique_ptr<Module>>(std::make_unique<BroadcastModule>()):
        (type=="%")?static_cast<std::unique_ptr<Module>>(std::make_unique<FlipFlopModule>()):
        (type=="&")?static_cast<std::unique_ptr<Module>>(std::make_unique<ConjunctionModule>()):
        nullptr;
    assert(result);
    if (result) {
        result->name = name;
    }
    return result;
}

const std::string eraseWS(const std::string& src) {
    std::string result = src;
    result.erase(remove_if(result.begin(), result.end(), isspace), result.end());
    return result;
}

SandSystem SandSystem::parseInput(const std::string& input) {
    SandSystem sandSystem;
    let lines = pSplit(input, '\n');
    for(let &line : lines) {
        const std::regex regex("([%&]?)([a-z]+) -> ([a-z, ]+)");
        std::smatch match;
        let result = std::regex_match(line, match, regex);
        assert(result);
        auto newModule = moduleFactory(match[1].str(), match[2].str());
        let destinations = pSplit(match[3].str(), ',');
        for(let &destination: destinations) {
            let trimmedDest = eraseWS(destination);
            newModule->destinations.emplace_back(trimmedDest);
        }
        sandSystem.mModules.insert({ match[2].str(), std::move(newModule) });
    }
    for (let& module : sandSystem.mModules) {
        for (let& destination : module.second->destinations) {
            if (sandSystem.mModules[destination]) {
                sandSystem.mModules[destination]->initialize(module.first);
            }
            else {
                // give it a dummy module to reduce number of edge cases
                sandSystem.mModules[destination] = std::make_unique<Module>();
            }
        }
    }
    return sandSystem;
}

std::tuple<SandSystemUPtr,int,int> SandSystem::process(SandSystemUPtr sSystem, const int64_t tick) {
    for (; !sSystem->mLivePulses.empty();) {
        let [source, pulse, destination] = sSystem->mLivePulses.front();
        sSystem->mLivePulses.pop();
        sSystem = sSystem->mModules.at(destination)->receivePulse(std::move(sSystem), source, pulse, tick);
    }
    let lowCount = sSystem->lowCount;
    let highCount = sSystem->highCount;
    sSystem->lowCount = 0;
    sSystem->highCount = 0;
    return { std::move(sSystem), lowCount, highCount };
}

int64_t doTheThing(const std::string& input) {
    auto sandSystem = std::make_unique<SandSystem>(SandSystem::parseInput(input));

    int lowSum = 0;
    int highSum = 0;
    for (int64_t i = 0; i < 1000; i++) {
        // press the button
        //std::cout << i << std::endl;
        sandSystem->sendPulse("button", Pulse::Low, "broadcaster");
        auto [sandSystemRet, low, high] = SandSystem::process(std::move(sandSystem), i);
        if (hack > 0) {
            std::cout << "rx tagged at " << i << std::endl;
            break;
        }
        sandSystem = std::move(sandSystemRet);
        lowSum += low;
        highSum += high;
    }
    return lowSum * highSum;
}

int64_t doTheThing2(const std::string& input) {
    auto sandSystem = std::make_unique<SandSystem>(SandSystem::parseInput(input));

    int lowSum = 0;
    int highSum = 0;
    for (int64_t i = 0; ; i++) {
        // press the button
        //std::cout << i << std::endl;
        sandSystem->sendPulse("button", Pulse::Low, "broadcaster");
        auto [sandSystemRet, low, high] = SandSystem::process(std::move(sandSystem), i);
        if (hack > 0) {
            std::cout << "rx tagged at " << i << std::endl;
            break;
        }
        sandSystem = std::move(sandSystemRet);
        lowSum += low;
        highSum += high;
    }
    return lowSum * highSum;
}

TEST(Aoc20Tests, TestTest) {
    ASSERT_EQ(0,0);
}

const std::string sample1 = 
R"(broadcaster -> a, b, c
%a -> b
%b -> c
%c -> inv
&inv -> a
)";

TEST(Aoc20Tests, sample1_doTheThing_32m) {
    ASSERT_EQ(32000000, doTheThing(sample1));
}

const std::string sample2 =
R"(broadcaster -> a
%a -> inv, con
&inv -> b
%b -> con
&con -> output
)";

TEST(Aoc20Tests, sample2_doTheThing_11687500) {
    ASSERT_EQ(11687500, doTheThing(sample2));
}

const std::string puzzleInput =
R"(%bc -> ph
%hr -> zq
%sn -> sj
%df -> dn, hf
%lp -> dm
%lf -> sn, vk
&fv -> sq
%gd -> vm, gr
%jt -> vm, lz
%xf -> vk
%nf -> lp, kb
%dl -> nn
&sq -> rx
%vb -> vm, lg
%zq -> kb, fl
%fk -> fz
%gj -> pq
%jx -> jh
%pq -> kb, nf
&dn -> kg, vt, tg, fs, pn, jx
%ln -> dn, fs
%fz -> vm, jt
%fs -> df
%dm -> kb, hr
%ds -> kb
&kk -> sq
%tg -> tv
&vt -> sq
%fl -> zl, kb
&vk -> bc, sj, jd, lf, xr, sn
%jd -> pp
%tv -> dn, pn
%sb -> gf, vk
&kb -> kk, gj, gt, hr, lp
%pp -> vk, bc
%pn -> pf
%zc -> vm
&vm -> dl, fk, nn, fv, gd, lg
%rn -> dn
%gr -> vb, vm
%sj -> kq
%zl -> kb, ds
%lz -> vm, zc
%jh -> dn, ln
%pf -> dn, jx
%kq -> sb, vk
%ph -> md, vk
%nc -> gj, kb
%kg -> tg, dn
%hf -> dn, rn
%nn -> fk
%gf -> jd, vk
%lg -> pv
broadcaster -> gd, kg, gt, lf
%gt -> nc, kb
%pv -> dl, vm
&xr -> sq
%md -> vk, xf
)";

TEST(Aoc20Tests, puzzleInput_doTheThing) {
    ASSERT_EQ(0, doTheThing(puzzleInput));
}

TEST(Aoc20Tests, puzzleInput_doTheThing2) {
    ASSERT_EQ(0, doTheThing2(puzzleInput));
}
