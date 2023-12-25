#include <gtest/gtest.h>

#include <pex.h>

#include <graph/stoer_wagner_min_cut.hpp>

using namespace pex;

//using MyGraph = boost::adjacency_list<, vecS, bidirectionalS>;
struct edge_t
{
    unsigned long first;
    unsigned long second;
};

using Vertex = std::pair<std::string, std::vector<std::string>>;
using GraphInput = std::vector<Vertex>;

GraphInput parseInput(const std::string input) {
    cauto lines = pSplit(input, '\n');
    GraphInput graphInput;
    for (cauto& line : lines) {
        cauto words = pSplit(line, ' ');
        std::string vertex0name = words[0].substr(0, 3);
        Vertex vertex{ vertex0name, {} };
        for (int i = 1; i < words.size(); i++) {
            vertex.second.emplace_back(words[i]);
        }
        graphInput.emplace_back(vertex);
    }
    return graphInput;
};

// boost graph library can probably handle strings but I don't know how and am too lazy to learn
unsigned long triCharToLong(const std::string& str) {
    assert(str.size() == 3);
    return (str[2] << 16) | (str[1] << 8) | str[0];
}

using Vertices = std::vector<std::string>;

int64_t char3toVertexIdx(Vertices& vertices, const std::string& char3) {
    auto vertexIt = std::find(vertices.begin(), vertices.end(), char3);
    if (vertexIt != vertices.end()) {
        return std::distance(vertices.begin(), vertexIt);
    }
    else {
        vertices.emplace_back(char3);
        return std::distance(vertices.begin(), vertices.end())-1;
    }
}

// it looks like boost requires vertices to be numbered from 0 to n, so...
std::pair<std::vector<edge_t>, std::vector<std::string>> flattenGraph(const GraphInput& graphInput) {
    std::vector<edge_t> edges;
    Vertices vertices;
    for (cauto& v1 : graphInput) {
        for (cauto& v2 : v1.second) {
            cauto v1idx = char3toVertexIdx(vertices, v1.first);
            cauto v2idx = char3toVertexIdx(vertices, v2);
            edges.emplace_back(v1idx, v2idx);
        }
    }
    return { edges, vertices };
}

int doTheThing(const GraphInput& graphInput)
{
    using namespace std;

    cauto [edges, vertices] = flattenGraph(graphInput);

    typedef boost::adjacency_list< boost::vecS, boost::vecS, boost::undirectedS,
        boost::no_property, boost::property< boost::edge_weight_t, int > >
        undirected_graph;
    typedef boost::property_map< undirected_graph, boost::edge_weight_t >::type
        weight_map_type;
    typedef boost::property_traits< weight_map_type >::value_type weight_type;

    // define the 16 edges of the graph. {3, 4} means an undirected edge between
    // vertices 3 and 4.
    /*edge_t edges[] = { { 3, 4 }, { 3, 6 }, { 3, 5 }, { 0, 4 }, { 0, 1 },
        { 0, 6 }, { 0, 7 }, { 0, 5 }, { 0, 2 }, { 4, 1 }, { 1, 6 }, { 1, 5 },
        { 6, 7 }, { 7, 5 }, { 5, 2 }, { 3, 4 } };*/

    // for each of the 16 edges, define the associated edge weight. ws[i] is the
    // weight for the edge that is described by edges[i].
    cauto ws = vector<weight_type>(edges.size(), 1);  // all the same weight in our puzzle
    //weight_type ws[] = { 0, 3, 1, 3, 1, 2, 6, 1, 8, 1, 1, 80, 2, 1, 1, 4 };

    // construct the graph object. 8 is the number of vertices, which are
    // numbered from 0 through 7, and 16 is the number of edges.
    undirected_graph g(edges.begin(), edges.end(), ws.begin(), vertices.size(), ws.size());

    // define a property map, `parities`, that will store a boolean value for
    // each vertex. Vertices that have the same parity after
    // `stoer_wagner_min_cut` runs are on the same side of the min-cut.
    BOOST_AUTO(parities,
        boost::make_one_bit_color_map(
            num_vertices(g), get(boost::vertex_index, g)));

    // run the Stoer-Wagner algorithm to obtain the min-cut weight. `parities`
    // is also filled in.
    int w = boost::stoer_wagner_min_cut(
        g, get(boost::edge_weight, g), boost::parity_map(parities));

    cout << "The min-cut weight of G is " << w << ".\n" << endl;
    
    cout << "One set of vertices consists of:" << endl;
    size_t i;
    int sum1 = 0;
    for (i = 0; i < num_vertices(g); ++i)
    {
        if (get(parities, i)) {
            cout << vertices[i] << endl;
            sum1++;
        }
    }
    cout << endl;

    cout << "The other set of vertices consists of:" << endl;
    int sum2 = 0;
    for (i = 0; i < num_vertices(g); ++i)
    {
        if (!get(parities, i)) {
            cout << vertices[i] << endl;
            sum2++;
        }
    }
    cout << endl;

    return sum1*sum2;
}
TEST(Aoc25Tests, Testtest) {
    ASSERT_EQ(0,0);
}

const std::vector<std::pair<std::string, std::vector<std::string>>> sampleInput =
{
    { {"jqt"}, { "rhn","xhk","nvd" }},
    { {"rsh"}, { "frs","pzl","lsr" } },
    { {"xhk"}, { "hfx" } },
    { {"cmg"}, { "qnr","nvd","lhk","bvb" } },
    { {"rhn"}, { "xhk","bvb","hfx" } },
    { {"bvb"}, { "xhk","hfx" } },
    { {"pzl"}, { "lsr","hfx","nvd" } },
    { {"qnr"}, { "nvd" } },
    { {"ntq"}, { "jqt","hfx","bvb","xhk" } },
    { {"nvd"}, { "lhk" } },
    { {"lsr"}, { "lhk" } },
    { {"rzs"}, { "qnr","cmg","lsr","rsh" } },
    { {"frs"}, { "qnr","lhk","lsr" } },
};

TEST(Aoc25Tests, sampleInput_doTheThing) {
    ASSERT_EQ(54, doTheThing(sampleInput));
}

const std::string& puzzleInput = 
R"(cxq: nfp chr dzz ljr
rgp: qbd dxz gjx llm
crr: pkp
cbm: mpk mfj
xtl: rqp dgj nxf bcr phb
nsj: zfl
bjl: dnj
nvj: jpn
qhd: tcs vgk mjs tjt mlf fbh jfb
dtk: zdf dsb xtn czh
htd: kgm vtm
shr: szp zhz
jhs: dhv frb jhh
csm: fnb qjp rnm
lqp: ssv qjj gdv vqm
rkd: ltx fdx xhj gpx
tgt: pbb vqt
cvr: zxx cgt
zld: rdg
dtf: rxz xjp xks vbt
hqx: ntz nnd
fpz: nkd
pmp: smm fgx
xpf: dkp gzg
qjx: tgq qkf
tdq: ngf
xds: rdk vdn zhx gvn
llh: ssk pqx tnn
cqb: rdg
zds: gzg tsj
dbr: nxt grn dkl bcr
zgk: xls hzr
ckq: rbh hks
mzf: zcm qjj czj cvj
lfr: xvq cdj gxl htv
gzh: gdj
hjk: rpj fsf mkk knx
jhx: lxc fsc tbn
mtf: vxd llg nct sxg
ftl: fks
xfp: cjt
ddg: cbk qmv
qmk: rjb pmb jjf
rms: nnb nqt hzv jll
lkl: tlb gcg brr hpq
rdl: zgk pqt hjc grl
knb: kvk lck mst fhg
vkb: zqt jjf qrd mdz bfp
jdk: snl gpx rqs
lsd: zjk xnq psf xdl
rlb: lck sdj
mmm: btm vxd svg qcn
dsf: tcl tfq
qql: dxc pxl xxh mvp
dgz: fvh hsl kqr hlc
jlp: flj dsf nbb
szp: kzr
gst: crj llh rrh xtn
zkz: bgr xzj jxv
jjh: sch cbm spl rqp
rhg: rjz
kcz: pvd pjq jbs gxt grh flx
mmn: nrr gtc zlz bnm
pxl: zfl zvz mgk
drn: pqg fpq bfj kdr
vpb: tbn
kbm: vjn tzg
pmx: mbj
vzt: rcx tlb
gnl: kcm
brj: fth zkq pbt gpd vth
vmf: nnj znf
dmd: dgs pxc jdt
hlc: sfc
jqq: zln tdq
rxc: mtk
knt: fqt pfs ztz bnb
qsk: cxd xjx
qgq: nxf rkz gsr lnq
gfj: ngh dmd stj
dfx: qqz cnc ckt
nbn: cft gpp
xqq: ftl jtk frb hfx
nqr: glh njb hrp hbn
lhj: krc hlx
xvj: tkc qmv
mln: vvb
hqc: gdv xjt nkr
btr: dkc hrf
tlp: rfl tqc tgk kpx
mmz: ngh zjk
kgv: rrh
ffm: vtm krb rqn xfp
zpf: lbj
bjq: jch bjd
kqc: qpv gkq vgk rld
rss: dxt glf nmp ldl hgg tdm vvt
cnz: xck pnx xqg
rnh: dcz xnv cqz cht
rmg: mzm pnl zph dpt
pfr: zkj tzf djz
cdq: dhv mdd qkf btp
msq: fps jvz nhd jzs
qkk: jpn kpx bqv vfd bgd
fmr: mcl rsj cpf
vff: hkq
gsb: hpq gdj xjp
ppx: cpt tjt hkg tjh
vlf: vzf vsn
hsm: bps bcc
xmc: mvm zhp njb rmd
rvv: tbn rrh rnj nsj
zrq: cpf jzx lnl
qcn: dkp rpq
ggg: dzh hxd rqp
sjg: zdk kzr fmv
zbn: hgp ltf grh
zlh: hzm jrb tbn
xjt: hgk ngk
zbx: htv
mqz: zkq tkk jjf xsn
pfg: snl
jjg: rhk qtb gsb tgt
mdg: szq rxc jbz sft rlg szp
rtc: hbs ftk fpt jmv
gjr: tbx
gxl: fbh bxk
hgp: qpp
pdp: kpr dmp cjt nlt
gnb: qzj
rqv: hxx
fxk: nxq
lzf: dxx ttj lhj zkq
tzm: dkh llg mtr bpk
dmn: xpk rzn pjk kbm
fnv: ztt jpl gxb mtr
sfd: gdj mrq qrd
ndj: cvj txf zfh mtd
tfh: jsk pdk gxn dgb
xpp: zql vzh xhm
hmv: rcj kfm mkk
shg: cnl stt
nsr: hrp hbx
mtk: snz
mbk: nxt plq
tzf: plv mhj ppp
xnn: qtb cht ngd
cms: ktz
rml: kdj zhz
nsp: rhb
mdd: vqp kdj fqx
tvx: zvh vcc tdq mfh
qtl: qkf ltr jfg rjb
hml: fth xrx
ftq: hhj dcc jkf
ntc: hkc pnl rcl qsq
dvm: hqp
mbf: qjt cnl tnn fnq
mrd: rqs spl
ppf: qgz xpx fbh
ths: cds jnc dcn
brl: bjm mrq
gpp: mrq tqc
mfm: lqg mbq shg
dnl: fhz btm nfc tgk
hld: tvl rpn snz
xmp: nsp xvz gcp
dxz: rtl mdz hnf
rtj: rhb hrt
xxq: jlk snh qtr vlg xdq rxr
jzq: mvf vrl fxb
svg: rtl kkl hpq
qjt: xvn jdl vqm
nnn: vkq mzm gpb mvv
dqf: fsb jrb qjp
fxh: lgp xps cnt knq
jhl: vqp
ngm: thx chd
dpx: kgr nnb
pjk: ksk mbq lqr
ffp: xqh cpt cxv
pkc: mcz
rsn: dsf ppd lkl
qmv: jxv
vts: tkk jfg
bpx: ntv lqd ltr vzf
scm: cnc rzt
gzl: zhq vkq
kxc: gbp pqq chr
gxb: xtm
kpx: rhk gcv cbq bfj zxx
tfr: dmx bdl gmb
pgk: qvm kzv ddr xjp
bxr: ngx dfx jdp
czs: fxb tkg
zlz: qjj
npt: fxf hxx dvm
shl: jvd tjt bmh
cqs: lmn hks
dzh: tlc
zrp: nkr mrn djj rhl
flx: zrh gql
gpx: ppq
zvz: kxt rcj
nmz: tdv ntv vbt
jbp: bgr gcp
qfn: jfb qns mhq dfl
sdb: jvd
stm: mrj
dpf: xtm vzt gqx
hjq: vlh bvq
hbn: gnz mkv
xpj: fpz
zlt: tzv zql pkc dzd
vnm: pmx cck jpp vts
dgj: fnq nnp
jkc: thq rqx fhj pfd
dnj: ggb rfp
kvf: gnk cnf klp vxv
vxp: rpn
nlg: cbk zvt
ztc: nbb jhs vbt
mpk: hsj
kxq: xhn vcf
pjq: tzg ktd
rnj: bbb
njf: jjq xtd bxq blq
tkh: pff slj
bsq: cdj
blq: rnm btr gdv ptj
cvz: hqz msx lqr
cqz: ljn ztz fth
zft: tfb tdn mbk
pxq: gnk vqm
lgr: lhj zgc rtk
bqk: tvl jnq nsp
cfb: znp bxj cgt
lsh: dhv mxq zzb
xck: gnk
gxt: grd
dcn: ctg
hxx: fjd vdt
vlt: ktj lgp stt
gdb: phd gbp mrf vts
kmk: gtn cts vjn qcp
tlb: rlg
pvk: vpc rfx xtz
gqg: fpf tbx
lqf: tvl lfc ngq gxb
qkb: ngv
qnn: hmp qpb
mlf: grs
xhb: qqd krq zqt
lzs: thq
cqd: rmm plv
pbv: vmf lxl rhk xhb
jvx: sxc pgx xtc hkc
ngq: pff
fph: cgv jdp slr jdk
zns: xpp clm ftl cqs gdx
psd: hkc ngv pvk mkv
ngx: jxm mcb
vcf: xmq nqz
gbx: kbd ftk vdm kvd
zhz: dcz
lrz: rxn stj
spk: dtk vjn bhm kcf njb
ftr: jcf srn
ptd: qrz nkn xjx scm
kgm: qmd mhq
bkk: qrv nrr rjf
gjv: fcc bxq
crj: qfq
ngh: vsn
ltf: mjm qhj
qjp: ktj
gxh: gql hrl
gvn: tkg ngp
zth: vkq bhm kvr jbs
mtj: zql vxd prn
gtq: qhj xlp mff
xrx: ccl bsz qmj
cxd: zsr
sfc: dgj xtz
dvx: qhj
hhs: tfb kcp
lhs: kqg vkq pkj mpm
jmk: fpq nzc sxg lvt
lpv: xxj rlg dzz
xtn: zrh qgx
gzm: dcs jdp tfj
cnj: dzz ssf dkj vzf
pqx: nqs
knq: ssv cnc
qsb: jlk hqc tfj bhm xzb xgs
gtc: zrs mln
mrf: tfq hfx jvz
jxz: thq mzm thz
vlv: zlh hmm cmg tgb
hjc: gpb fsk gnl
vgm: pzl kxt
hdb: hkg hgk snh glx
cjz: rrs rtk zkl
hkc: xpj
dhp: fgn lzs tdn dvx gpx
ddt: jsq rjm
clm: hzb
vth: jjf bfj
nrx: tdt rsc cfr kxq
bkb: zxg ztz szh
crk: sgs tlj bqv xmj zjm
tmx: ckg vgm rdm rcm mhd
kbd: mvm smz xtc
qld: jnv gpf zrh xgs
jfb: pvb hmm
shm: dgb pdf
mff: bvq
pdh: bqk ztt zkl dns xch
nmg: bfg qmd tbb qcp
vnb: bsz mmz klj vkf nfc
xxj: ljr jmt txt njd
dns: plv
ccs: lpx djn hxh pcj cnk
fsc: stg
xnv: fzp
xxh: dgr kxt
vtb: mjm
dzq: plv dkj lhd
jxn: xzr jkm hkg
jmp: gvb hqz ths cnt fnj
tck: vzh tpd tsj
hlx: rhk vrl
hlj: hgk
nnj: tbx
cts: mln fqn thz
qsq: ckt
fps: xgk ngp
ffg: kpt hgg
czj: vvb jrb pfg
xzv: pvb fmr gql
qhf: hgg chd jdl
nfh: rrg crj hqh mdh cbf
fmv: lck brd
qsr: qgz cml bxq kvr
bjx: brr npg trc lxl pzc
hmm: sch
mph: fhj jbm hsz
xqm: fcc sdn vfm vxc gsx
kxs: dnf jqm vcp zht
lzx: pmp xpv
fxr: xvl vbv dmx dgs
pkp: dns
pbz: nfm chr bxj ddr
fcf: qmk vzt fxk dzb qbj
djj: ldb vxk
vqq: dgm qgm
xlg: fxk ssf lhd rgj
vdn: mxq zmj
cdk: jgp njf qtz
ffb: cfb rtl jlp ztc
ppn: psp scm cjt
zsp: mpm knx hzr hbx bff
tgb: vqm qtr
rfd: dhp gxl ffm
hjv: fdx fsc mfm
kcm: gql
cck: gzv fmv
vfg: bnm pnx vnp kfm
lfx: sft bqh qmj dzz pqq tkg
sfq: dkc xgs bhh xvq
rsz: kqr
jdl: cnt
ntn: zqb stn xhn fhz
lqb: xvl vxj qtl vkf
snh: mch
hfg: lgr xnn cjf mdt bsz
zqt: ghk nmj ljr
rfc: rml lzk xvz kxc
tbb: nxf
dsk: jfm nhg smm hfv vjc
fsk: hrl vng
zfj: gqg tkj fgx mbj
mxn: dkl cxs zcm dsb
xfg: gnb dkc dhp zhp
mqj: mdt jbz
dbh: qgd vqm
zhr: xmd mhm nkl
ccf: qlr rld psp hxc dxc
grh: nkd
snb: smx fpp slj
zln: nzc
bff: tbk cpf vff fcc sss
dgr: fsk blk
rzn: jch
dcz: zql
pdk: hpq nnx
rqn: fnp
xtd: hrl qpp
qhn: pkc ngh snz
qmf: ppp rgj fmv mkp
tld: cfl gpb
sss: rnj kvr stg jgp
vkx: tbm hbs dvx jck
gcg: nmh bjl clm
hhm: qlr bvc
pvd: gxt pkj
nfp: jcf
czv: mdt bdr
mgt: kxc mhn pmf bhq
lcj: pkp mtj
pzc: gpd stn
fnl: mlr qff dtd
lmv: gdt xqq sxg
lnq: rcm
zdl: xtc jvd jvx mtd
blx: hkq
kgr: zrh
ppq: bhm
gpd: cms
jpn: qgl bjl
gmb: xmt nnj rtj
kvl: vlt cnz nxf rdl
pmb: snz
nzc: hrt dzz qcn
vpg: npt vkp csd vjv
rfl: cmn
dgs: rcx
bfp: rkg rcx
zsr: sch qrz
jft: lvp zft blq pxq
znf: gjx lzx mfr nqx
ccl: cbq bgr
vkp: nqz ngp
hkd: mtd tbn ppq
qbn: jxt jsk hvz
jmt: dcp rhg hkl
nkl: xpn
qpz: jgc jrb vnt xpj
zzm: knx smz tjt cjc qgx
zpc: bhq ccp gjx pmx
bzt: qsq fcc zxc tdn
gnm: vxj rkg pmb qmz qhs gsk vtn
bjn: rxn
tdr: dnf zmj smx
hsk: dzd tkc dgb
ppp: zld
ktz: vdt
qhh: jxm kxz thz jkf
jvl: jfm csd rxj jhl
njb: gpm
zvf: rcl vxk jjq dlb
zkt: pzl mcr gpf
nkc: pzm zkn cms dkp
zmc: ckg qtr
bcc: xtz bxg
ptj: fnb
rzv: ccl jsq
vpl: htv
jzh: czs shk bhq lhj
xnx: zzm bxq cjc lkx xxh
jdp: mfj
hmp: qdt
zkg: xpx
tgc: htd qql gzl vnh
kkj: fdx pvk zpv
vbt: ljn jvz
tjr: qnx hbs bkk zpf kkn
txf: kcb
dch: mlr
lnv: vxc slq hjv
vgk: crj
tgx: jtr zlz
vxk: vlg
hvb: cht
jpq: cft tlf fgx tbp
nfm: cfk sxg tdv
cbq: rhb ttc
ksc: bhh rzn ljz
hxh: mlt gxn tkk
kqf: bdr dcz pjh vcf
zkr: fxf lzk jtk xks
tcf: lnk
djz: nqz gmd
jgf: jqq mkp gjr tkk txt
dzz: hvb gbp
cgx: bqn lqr nxz qsk
lkg: mhn mck smx
dcp: kvk jtk
vrf: zbx xlp
rqt: dmp dfl
ppd: fsn xpn
cjx: kkl lhn xgk zvt zkj
fhg: vkf ttj xpn
mbc: xmq sbn
jpj: cjc zph fgn ckg
pnl: xpk qgd
btp: shp zxm
kkl: lzk sjg
bmh: sbj grd dpt
ndl: rqx rzt hjc ctg
mjt: jll
cbj: cxd
mdh: dpx qns jfp
krt: pgz
pqq: kzr jqm
bkg: qgl sjj dvz
fdh: cvk pll
mvf: zdk
cjj: zqt cfk bzg
pfs: ngf
dhk: lhg rsz mfm
cvk: flj mhn xpf
bzg: ddg
xhm: cqd sxg zln
sqx: xjf gsk mfh hvb
lqg: zkg bfg
fxf: bjm gxn
txr: bvc szd snl
cdh: qjg nmp gnl gtp qtz
dbl: nnp fnb vvb hlj
gnh: nrh
phk: hsm jkf qsq
rhl: vgk nnd pgz
vtt: ftl fnv pch psl lxl
kxt: hbs
ljk: cts mdh cjt lqr
kpr: qkb qgd
jgc: fnb hlk blq
cgt: pls
nct: cbq kbq
bql: jjf nnc fhc rjp hbj
xvz: jhl ntm
shf: zjm hfl qcn dmx
mck: nvj zzb
drx: lvn gpb jtr sdb
vqn: ssk
rkg: hpq dnj
ltx: mjs
brb: mtk fzp kpx ssf
zjs: mvl zfl cml
bcr: gll
gmv: bpk rkh lgf vmf
nmp: tbm
jrn: rnq xlb rls qbn tzv
qpb: zzb
fhz: zkl
ccz: qpg kgr tjl gqd xfp
plv: mfr
zrl: nbn cmn lkg nzc
nkr: cmg
vxn: fsf cxd vqn
xvk: qmz fvd gqg vfd
nbk: pqx mhd jbl zlz
thb: rfx qzj nkd nzh
kcf: ksk dgm rcl hkq
jht: kcb gpf zpv jdp
slr: lnk
gtr: qbh grd jtr pfd gnh
vnt: nhm xmn
ftk: vff fqn
xvn: hgp cnt vpc
pzr: qnn gvn
zhp: kcb
dtn: gll kpr ffp sbk sfh tgx
dlv: ptj jjh dgm
kqs: fnl nbl nsq zzb ppz
vnp: mmn jpc cbj
flv: jsk shc vlf qcf
jml: bqk rnn jnq cqb
nkh: dlg vpb hqx rqn
gbn: lqk lgp zxs cmb
rcj: dgm rcl
jxl: xdk
nhg: zmj tgk dkh
nrp: dgm
slj: jhh jsq
lxc: xzr
rsj: bjd
fsn: cmn
bjd: jxm
vjv: dcp frb rfl
ggm: qpg ngk
rls: dhv
xvq: ktj
ffh: ssk cnf
qtb: rpq
jmf: mjr kkj gll fpt zrp
bjp: ttr zjk
mcp: mmz snb vqv ncv rdk
lhb: bkb vth gmv zxm
crb: xhj lqr
jkb: xnv rcv qdt lck krq qll
jll: xdk fnq xpk
glx: zhp fhj bnm
grn: vdm tdn qtz
cfl: kcp xrq qqz
kdv: jsl zdf
xzz: tgq rpn qmj
qlf: lzf zxg glv ppz
vzm: rtj cqd npg
lrc: zmn vdt
stq: sch zfb bqn pkj
rqs: cpt
ncv: nmz hnk ghk prn
jjv: nlt sfh bvq
zzb: hnf nmj
ktl: vrf jtr vxn shl
jsn: nkn nkd cmg gtn
tgq: krq
plp: cvr tck pzr
bpv: plp vpm ppz pzc mrq zxx
pqc: qfm hlz lhb
xmq: cvm txt
bmv: xvz xtm
hqh: mvm zpf
)"
R"(thz: tmk
vxz: rsz xqg tjh hgp zbn
qjj: qrz rfx
vsg: smx zmj tbp dns
sgs: hsk pmx fxv
kpk: gqx smm
qhq: ccp tsf rdg
gzg: stm
hsj: jkm
nqx: nrs tvl zhr
qgx: gpm
dnf: hsv rtk
pmr: dtd dvm hdd mhj
mrs: ftr czv
hkl: tdv vjz
jqm: bdr
hnk: hmp trc zqb
kgd: dsb vpc rrh hhm gpf
dcs: knq vtb qvv
mhd: jbs dgj vdm
mkp: mtr
zqp: xnq xhn fks hml txt jfg
qkg: jxl vff
zfb: kgr
dbx: dvm vbv xvl ktn
vxv: hgg dmp dvd
hqz: qvv
qcp: glf vqn
vsn: jsk
lrh: djz spd cqb
zdk: zld
vfj: ddt vxp bqv
kms: ptj gnk dcn cgv
npz: lbj thq fnp
xqg: rnj
xdq: qkg gpm sfp
tkc: qhs prn
qqj: glh cds zpf tgb xvq
fpv: qhs vhm gdt fzh
gsx: bsq dcs qhj
vnh: txf kvd dxt cnf
fhc: hnf zmn
qbj: bqv mst
qgd: sdb nrr
vhn: spd tck nxq pdf rjz
vlh: xjx
zgc: crr jzs jhl lrz jpp
rjz: zxm xmt
dqr: kxz
ltr: pff vdt
bvc: czh gkq
xls: mcr
gtp: xck ntz pnx
hfs: cgv hnp
kps: tsf bqh pls mlr cvm szp
ljh: fnq gvs tns
mvz: nbk hxd hfs lvp ffg jgp mbt qjg
fhp: qvv gvs htd
zvh: mbc bfn tfn pfr
bjm: shc gjr
krc: znp
hkg: mrn qns
klj: kzm vdn tkg
lpk: rgc fpt jzx hsz
tsq: dvz zqb xsn xvj
jzs: sbn frb
ssf: ktz
gfk: tdv fzt vzf
pqb: csj szq qll jfg
zkj: jqm
rjm: shc
frl: hqp bxj xvj brl thx
nzs: ppd rlg rzv
hmk: xnv jpq mgc kxc
qgr: xsn qqd
cfr: ttj
kfv: tlc zkt cjc hjq lhg
rgt: rcv
hnr: kzm rqv cgb lzx kdr qhn pmp
tjl: rkd rcj xtd
fms: qpb
lcd: fvh hsj tns
xpv: xmd lhn
rfh: rtk tsf
flj: zhx
xjf: ddt zdk qrd mlt
bfn: nnx nnc
lqd: lxl lhd
cgv: phb
cml: nkr dpt tgx qkg
tds: lvn zfb vxk kvr
hxc: rgc nzh
mmc: ffh mjt nmg
kjq: tdt bkg
tdv: xnq
tls: nct
kdk: zfh mvv ggm ppq
rnz: hcp mqj hsv fsn
fqx: xpv
mgk: nkn
vbz: qfm vsn rhb gqx
hbk: znp chz
ssv: zrh zbx
vqt: dhv znp
mlt: xdl
jnc: vlh
gsk: mck dpf
qpr: sfc kpt zfh
xzj: fpp kps
xbk: bfg ggg bbl gtr gxh
xkp: krc xzj vlb
mnd: nrh qgm xpk jxm
ctj: dqf mfm nsr dcn
qzz: mkk pvb xtz
bbl: mrd xpk tbn kpr
tbj: bqd qkb gnb cph krb
bts: vzf vpm shp llm
prt: zln vlb hrt sjj
gnr: fgg lnq flx czh
ngd: kps qhl bdr cht
zsj: cdk tbm jjh mcb kcz
mhr: qgz zkc rfd prv
lgv: hfs kqg fvm
vng: stg mjt rnj
kml: rnq ngf dvz
dxj: mvf llg zln vbt
lpm: vmf lrc rkg ckq rxn sgd
pzb: gjr rqv gxb
nqt: jlk hgg scm
lmn: vlb gbp sbn
zkc: hgk
sft: mcz mfr
tdm: lzs mhq ndl fhp
rsx: nzs sjj vnm nbb tgt
sqh: zkg gtc blx cvz
tvk: plq pxq ggm qgz
zmj: jvz
jgm: rfh gpd jbz xqq jcb
tlc: tfj tlv
pcv: jxl mvv zhq qrz
nmh: qhs
jcr: gpb lbj fqm zlv
zkn: zdj spd qdt
nrk: mdt qnn fqt nmh
fqm: rcm mfj
qcf: zxg nsq msb
lzk: zvt
qhl: cbk xnq xhn
zkq: fgx
chd: gnz
mrq: brd
dxc: qsq blk
vjn: ksk
gxx: bct nrs tkh
xhj: zrs rqt jgp
krh: lrh pdf cgb
tbz: nfp rlb xzz
jkf: bbb
pqf: ntk tht tqk tns
clb: lpx jbp fhc
vvt: prv hlk lqk
tkj: vkp qjx tzf
bbc: cms pdh zht qbj rgp
cvj: vqf dfc cmg
qgm: ckt
vtn: nkl
fck: nmh vmn
nrs: szh tpd
bmk: cgv vpl
vqm: qpg
prn: ddg
vdp: tlj nbn ckq dkp gfk czv
hrp: nnb pzl
kmz: xpn djn fjd dtd
glf: jbl szd
qns: gxh
jjf: dgs
rjd: jkc tmk glf hrf czh tqk
klp: tbf rsj mbk hvp
bqh: hfv chz
vkm: sbj ltx zdf kmx hhm
mfj: sbj
nvf: bjd dzh jvx bjq
gkl: rgj fck
kcp: blx
pzm: zld
blk: zpv mpk rqt qcp
bkn: pzm gmv mdd xmp gxj
vcx: nnd xrq grs sbj
slz: lsh dnf lxl fxk
mct: jsq hcp jcf vfj
mcb: rcm
dxx: gcv
vdd: nkl rxn zdj lgf
qjh: vqq jsl ntk cjc
hcp: jcf cck
gmd: vtn rxc
hvp: jhx tld
hfv: fpf
bsz: zvt
shk: kpk brd dnj
nbl: rnq ljn zkj mbc
bqd: ngv qfq nrh dvx snh
jmj: hkd bqr ntz czj mph mlf ltf
pfd: rcm nrr
xlb: dzq gxj qqd rjb xgq
bbs: bsq mpk
ggb: mbj
xts: bjl bqc smm nfp
dcm: cgt rlb fxv czs
tcl: zdk gpp
shp: txt
psl: rxj bzg
qbd: pfs hlx hkl
jcf: dnj jfm
mcl: nrp vlg jck
gvb: kgv hvp mvl
dqp: dtd rzv kbq spd fhz
hsz: dgr
lqk: ztg dpx qmd
mrl: gdt mfh lmn gzv
rdc: jjv jnv tch nxz hlk
tfn: crr qkf szh
rmd: zdf qqz qgx
lkx: nrh krt kzx jvd nnd
zqb: dxx qgl qhq hnf
dmz: czh mpm ntk hqz
tgj: dkl tfj qrz vxr
szh: txt
hbx: bbb
cgb: gdj qrd pkp
kqg: pfg jfb
krq: jfm
pcj: tbp vrl gjx
psp: qpp
qzj: qrz xtc bbb jxn
tfq: qmj rjm
mcz: nmj fzh
trc: tck
bjc: fdh fnm vsg zkl
vzh: fxb jpl jtk
snl: vff
brr: bgr vqp hks
jjm: kgd qjh jmf xzr
stj: jdt
sgd: zht cfr crs
tlj: jzq bsz znp
fnj: tqk lpk gnb
djl: vdt jhl ngf qvm rxz gmd
lgp: xpj
bqp: tzv tls stm vnm lcj
mkk: bps mfj
fsf: cpt rld
xhn: pls
mdl: stt bnv xpj qsq
srn: xtm dkp
dfd: hrl ffg xlp mmc
mvr: fsx lgf mqj cjz
glg: hlk
xqh: gzm dbh ktj
mkv: nkn
lbj: mfj
bcm: hjv zzm jnc xdk
xvl: vbv
fvm: ccp
nhd: fqt cfr mdz
gnt: gnl jch hsz mch
nsq: xtm
qkj: rfx jch cxd
sfh: ngk
vxc: nlt mcl
tbp: msb
dvd: zkg vpb jll
sxc: psd cjt xgs
csj: pjh nnc mtk lrc
jbz: bdl
rmm: vbv
zgt: hbk pmf znf gzh
nng: ngq zmn zkz
mch: bxb lqg zpv xls
jfm: frb
vfd: gds fhc
jfp: rcl mtd jjq
rrx: xgk zhz dtd xmt kjq
bss: tdq nkc njd fpv
hvz: zzb
xpk: nzh
fgg: xls kgv
zlv: ldb
lnl: mzm mrn qjj
nnl: vjz rtl ghk gkl
bmd: tlf dvz
sjl: nhn tnn bvs cpf
ttc: fpf kvk
fqn: jkm grs
rnq: lhn
hnp: xpx
rrs: jnq ljr qfm
krr: shl kcm ltx psp
pnh: tsq hvb dxx fsx
ngp: qmv
mcr: ngv
xtj: gvn gfk hfl pqb
tgk: vxp rbh
tlm: rzn fpt zvz qjt
nhm: mgk msx
ghs: vqm jmv cts
tlf: lfc zdk
xst: hzr bxb lnk fgn hzv
tlq: bbb
pvb: dqr hgg
kzx: htc xzb
mhq: gxh
hfx: hqp mhj
ckg: vvb
hmc: rzt jjh jjq zkc
xlp: tbn
bqn: gkq htv jsl
rsp: fhp vrf sfh
thv: pjh cpp gxx bmv
tbk: xrq vqn grs
hvq: ppn lgv zrq vtm tbm kxt
sdl: rsn rjp mtz hld
jpl: gqx
vxd: qmz
hng: ppf ngm blk htv
ngk: jbl jnv
shh: ggb kjq qfm rtk tpd
krz: tmk gtn rsj rld
gfd: rfh tbz slz btm rmm
ztg: tnn zsr
phb: kxz vvb
glv: vdt xmd
fvh: grh mcb
kzm: ztz ntm
rrg: pqx nqs kpt
ddr: kdr
fjd: bct
jnq: hks pff
glh: dqr gzl bjq dbh tld
rpq: xmd
xrd: tmk tgb qjp grs
cxs: gnz dqr glg
tqc: fpp
djn: szh
llg: fqx
rkx: cbj jbm gxt zlv hvq
fsb: gtc cnc xhj
jjr: kdj brl rxc bjp chz fpf
fnm: brl ntv vcf
qrv: kxz qkb qlr
cfk: cft
kbv: fqt fmv tdr rpq
fdb: kml hbk dcp zxx
tlv: jsl qfq
bhq: tsj rcx
fzr: kzr ftr cvk zkl gjr
pkj: nxf
nmm: fsn gfk rsc glv
jmh: zkg ksc qtr xps
dmx: mrj jdt
vdv: nnx shc sdl tkh
gsr: xqh hlc bxg
fnr: fpq pkc gsk zmq
ccp: ghk
std: xdk nqs xzb vpl
rnn: rdk fms ttc
hgm: psp xbk fqn nsj
gpt: kgv jbl jmv mkv ssv hjq nhm
lpx: zql
jtz: mrs xdl fzt fck qdt
lvp: cgv ktl
nxq: jhh
mnr: xck crb xqg zrs tbb
szd: mjm
pbh: nvj rls qgr pfs bmd lmv bjp
xmn: ftg xpk qns
sdn: vlh bhm dfc qpg
ldb: vxv jbm
dxt: krt vqq
prv: lvn
dpm: bhq hsv xsn xpf dvz lhd
dzb: rcv
kdr: dch
mhn: dgb
tvg: dkl xgb bps tch cxd dvx
tcs: rjr nrp rsz
pnx: mjm dsb
cbf: kfm jxl
pdf: frb
fst: dgr mrd hsm smk
rsc: dch
ftg: hqh
jxm: rjr
zrs: spl
mhm: gcv
zxs: jgp zhp lcd
cbk: pls
zxc: xjt gnh fgg
mrn: mcr
kgk: vxp qgr shm tfr pzr
hrc: bqn fdx crb vdm
dlh: fqx nbb gsc jcf
slq: htc jkf
fzg: mhj nmh lgf bmv
kxf: gnt mgk dmp xkh
gmk: dcc sfc mjd rkd
xks: crs
rjp: psf gpd
qlr: qtr
nfq: fxv nvj nng
ttr: krc tsj flj
cjf: shr bkg tpd zds
rkh: nkc zdj kdj
kmx: xqg zbx pgz
mvp: zkg qjg czh nsr
mvv: sdb
jtd: bmd sjj mrs mrj
hzm: zkc rsj qpv qmd
vfm: ckt kzx zmc pjq qpr
gcp: sjj bgd
mgc: ggb
pbb: pff vqv lfc bqv
qdt: zhx
zcm: grd pqt jdl
frn: dhp kcm szd nxz
mfh: bqc xtq
ntk: lhg
pqg: vqt gmb xmd mfr
hhj: jnv rrh zhq
kjt: sfd szq pll rtl
bnv: qhj
qcs: hnr psf bmv nfq
smz: grh pjn
tdt: lqd cft kps
ktn: fks zht hrt
gxv: qvm tkg qgl kbq vkf
dkc: sdb
vms: kfm qkj hsm nzh
nqh: tlq qgm hmv xjx
cnk: mbc xmt bfj
nfc: pdf pzb
dgb: hqp gbp
pqt: gdv qgz
fvd: lrz rmm sjj
phd: nsp szp tsf
jpc: mjs thx jzx lgp kbm
bpk: fks nkc
dsx: bjx bnb rsc gxj stj
qhz: fhj rqn dlg tbb ftg
mjr: kdv dgm gnz
pft: tcl vkp mlt nsq
sbk: mpm dqf dlg pvd
pll: rgt vrl ddr hxx
gkq: rgc
jzr: grd brq mdh
jlk: xtd
dcc: qqz lzs
npg: mhm
dkh: fpf
ldl: pzl fnp blx
ttj: hlz
ltk: shc rkg npg ccl
zsv: clm fzp rxj kmz
zjk: tvl
qfq: zhq
jxv: hlz
nxt: fvm vlg
cpp: nfp csd rfp
ppv: zzm bxb xzb tlq
gql: jbl
lvz: hmm qzz tbn dlv thz
nhn: cbm vqq vqn mbk
mjd: dfl fnp tns
pjr: grh msx grl gtn
vqf: kcb pnl vtb
zpn: xks ntm xzj qnn
nmc: slr npz rjr qkb
hsv: ktz
snm: jkm bcr gtq nsj
rbh: zxg cmn
tzg: vtm mbq fqm
jpp: zdj nxq
xmj: rhg ntv xjp
tgv: gzh shr pdk cvr
thx: cjc
vhm: kxc nnx
xtq: mcz slj fth
llm: gqx cqz lhg zhx
zks: rfp vhm qmv rls cbq vtn
hdk: xkp cjj btp mfh
sdk: nxz nxt bcc phk
gxn: qdt jsq
rjf: qsk jzx glf
dlg: dcc
mbj: brd
bjt: bxk qpp pjn
bls: pjh chz qhn kpk
hzv: zkt ntz
qtz: gnz
hqp: fzh
xlq: lnv tfj chd kcz kxf
xgq: qtb tbx rhg
rjr: hbs
smx: rjb
czb: bxr ftq mvl txr
jmv: kcb
gxj: hvz fdh bfn
gvs: nnb
pfm: pgz knx bjt hqz hnp bsq slq
pbt: ppp gsc bct
brq: czh ngm gkq
qll: csj mgc fjd
tbm: jbs
zmq: mlr rhk
gmq: slj zdk hvz stn
smk: ljz glg nnp
krb: gqd dzh xjt
sch: vlh
fgn: rzt qpp
mjs: xpx
kzv: dvz nqz bct
jtk: pmf
tch: bbs hlj
jxt: btm
nnh: pfd mdh kbd hlc
zjm: zmn snz
tqk: vkq nlt stg
crs: psf lhn hzb
hzr: hkq
gcv: bct
cnt: pzl
xps: qbh btr
mdz: qpb
fxv: jsk qqd bgd
qff: rgt llg tls
rqp: zpv
hsl: bnv hrf zjs vpb
grl: nlt
jdt: gzv mrj
tbf: gvs sfp hhs tcf ssk ctg mbt
stf: lfc pxc nrk bzg
rkz: fbh kgm fpz
gdx: vth mvf
nnc: nlg
stt: fsc
mbt: dfx pjn
tjh: qtr qhf
cph: gnh kcp hhs
qjg: fdx
xmh: mdd trc tdv tfq
zql: sbn nnj
vxs: gzh krc xnv tfn
kkn: hxd rqx
njd: vts xvz
fbl: pmb nbb lpv bfp
tht: bxq gnk mff
xhd: jzr hlj bfg vpl grh xzb
tsv: hml pqc lpx ghk shp
fsx: tck sdj
rgj: kxq
rhq: bjn brj flj nmj slj
vdm: dfl xzr jbm
vmn: dkh ztz fms
zxm: rqv szq cvm
tns: vkq
vcc: rdg chr nrk
bvs: ngx cnz mln fvm ghs
rtl: srn
cnl: bps dlg
bqr: txf jxz btr grl
nqs: lnk
ktd: zfh tcf jmv
dlb: kcm dhk bxg
kqr: kgv qbh
ljn: zjm
jcb: qff gxx cfk
sdj: gzg frb lgf vzh
cxg: nxt bbs hqx bjq
fpq: hzb pmb
tzv: dzb
xfz: bmk zhp hlc
plf: bjn kkl fnv nlg
bxj: cvm
grd: zfl
dbb: dch vqp xpv qmz shm rfl
rdm: hbn hbx lxc
plq: qtr nxf
pxc: hmp dzz pff
xch: gfj zds
ztt: tqc slj
fzt: mcz tlb
vxr: xrq bxb bxg kpt csm
nfl: rdk vjv kps bjn
jgz: ppp rxc tgq
hzn: tsk gdx ccl qvm
vjc: lcj tls fms djn
qpv: tfb zgk
kvd: qbh kdv spl
jkx: hxc htc xdq xzv
zzj: dfc tlq bnv thb
qnx: cdj rqs stt
lhg: jnc
lvt: pzm rjz cgt vzf
dsq: vgm nmp spk xhj
xgb: lnq ljh hxd
hdd: jbp stm msb jgz
mtz: xgk zql
vpm: krc nng
bxk: vdm qhj
cmg: jll
cdj: qvv mvm
bgd: smx
hpm: qhz jkm bvc qsq
bdl: cbq rpn
xkh: tlv ktd slr
mxq: psl rjm
ppc: gkl crr fpp fzp
rnm: mvl dgm
mzp: zmc djj kzz cnc
pgx: dpt vtb fhj scm
ppz: nqz
clr: xch slz jxt qjx msb
bqc: vlf zjm
mhj: vlb
tfb: zfb nnp ksk
vpc: ssv
ctg: hlk vxk
cjl: lxc smz pfg jcr
rxj: mst
cxv: cbj lxc vnt hrf
vbv: vjz
gdt: jqq
zph: rsp
vcp: zmq mtz kvk
tql: rqx hnp gjv tbm
gll: lnl
hbj: dzb zld rxz
vqv: jhh mst
dfc: mjt
bhh: xpk gpm
cmb: zlv bhh htd
psf: rcv
xqt: znf clb cqb mhm xhb
tjp: bkb jxt llg rfp
bnm: zlz
dzd: jxv hfv jpl
sfp: kkn msx jrb
qmd: tcf
hfl: hzb gzv
mtd: bvq jck
mkj: kxz xrd htc nhm
rpj: lhg pjn glg hsj
pch: mtr fxb hzb vzm
kzz: qvv shg fpz
bnb: mgc kbq
gqd: fvm krt
ntm: gbp
szr: tch xvq ffh tds
ljz: bmk rgc gjv
mbq: xfp
cds: mff nrp
gds: gsc vjz xdl
bvm: vkx xfz mlf vtb
dkj: knt
lhd: rgt
tsk: rxc pmf jpn
gsc: rxz
stn: xhn dkj
dzk: fps ngq csd cqs kxv krh rml
kxv: dns mvf hlz
jbx: lvn ftg zph cnf tld
vxj: mkp fzh
rxr: ztg jck cbf xqh prv
)";

TEST(Aoc25Tests, thingy) {
    cauto stuff = parseInput(puzzleInput);
    ASSERT_EQ(0, doTheThing(stuff));
}