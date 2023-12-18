#include <gtest/gtest.h>

#include <optional>
#include <regex>

#include "pex.h"


using namespace pex;

int64_t hashStep(const std::string& step) {
    int64_t hash = 0;
    for (const auto& c : step) {
        hash += c;
        hash *= 17;
        hash %= 256;
    }
    return hash;
}

int64_t doTheThing(const std::string& input) {
    const auto splitInput = pSplit(input, ',');
    const int64_t result = pAccumulate(splitInput, 0ll, [](const int64_t sum, const std::string& step) {
        return sum + hashStep(step);
        });
    return result;
}

struct LabeledLens {
    std::string label;
    int focalLength;
};

using Box = std::vector<LabeledLens>;
//using Boxes = std::array<Box, 256>;

void doStep(Box boxes[], const std::string& label, const char operation, int lensStrength) {

    // Each step begins with a sequence of letters that indicate the label of the lensStrength on which the step 
    // operates. The result of running the HASH algorithm on the label indicates the correct box for that step.
    int64_t boxIndex = hashStep(label);

    auto& relevantBox = boxes[boxIndex];
    //
    //The label will be immediately followed by a character that indicates the operation to perform :
    //  either an equals sign(= ) or a dash(-).
    //
    //If the operation character is a dash(-), go to the relevant box and remove the lensStrength with the given 
    //label if it is present in the box.
    if (operation == '-') {
        // Then, move any remaining lenses as far forward in the box as they can go without changing their order, 
        // filling any space made by removing the indicated lensStrength. (If no lensStrength in that box has the given label, nothing happens.)
        // rules of the game indicate there can only ever be one, so eraseif is fine even though it goes to end
        std::erase_if(relevantBox, [&label](const LabeledLens& focalLength) {
            return focalLength.label == label;
            });
        //boxes[boxIndex] = changedBox;
    }
    else if (operation == '=') {
        //If the operation character is an equals sign(= ), it will be followed by a number indicating 
        //the focal length of the lensStrength that needs to go into the relevant box; 
        //be sure to use the label maker to mark the lensStrength with the label given in the beginning of the step 
        //so you can find it later.
        // 
        //There are two possible situations :
        //
        const auto& existingLens = pFindIf(relevantBox, [&label](const auto& focalLength) {
            return focalLength.label == label;
            });
        //If there is not already a lensStrength in the box with the same label, add the lensStrength 
        //to the box immediately behind any lenses already in the box.
        //Don't move any of the other lenses when you do this. 
        //If there aren't any lenses in the box, the new lensStrength goes all the way to the front of the box.
        if (existingLens == relevantBox.end()) {
            relevantBox.emplace_back(LabeledLens{ label, lensStrength });
        }
        else {
            //If there is already a lensStrength in the box with the same label, 
            //replace the old lensStrength with the new lensStrength :
            //remove the old lensStrength and put the new lensStrength in its place, 
            //not moving any other lenses in the box.
            existingLens->focalLength = lensStrength;
        }
    }
}


//The focusing power of a single focalLength is the result of multiplying together :
//
//One plus the box number of the focalLength in question.
//The slot number of the focalLength within the box : 1 for the first focalLength, 2 for the second focalLength, and so on.
//The focal length of the focalLength.
int64_t lensFocusingPower(int64_t boxNumber, int64_t slotNumber, int64_t focalLength) {
    return boxNumber * slotNumber * focalLength;
}

int64_t totalFocusingPower(Box boxes[256]) {
    int64_t sum = 0;
    for (int i=0;i<256;i++) {
        const auto& box = boxes[i];
        for (int j = 0; j < box.size();j++) {
            sum += lensFocusingPower((i+1), (j+1), box[j].focalLength);
        }
    }
    return sum;
}

int64_t doTheThing2(const std::string& input) {
    const auto splitInput = pSplit(input, ',');
    Box boxes[256];
    for (const auto& step : splitInput) {
        std::regex pattern("([a-zA-Z]+)([-=])([0-9]*)");
        auto regexResult = std::regex_iterator(step.begin(), step.end(), pattern);
        const std::string label = (*regexResult)[1];
        const std::string operation = (*regexResult)[2];
        const int focalLength = (*regexResult)[3].length() ? stoi((*regexResult)[3]) : -1;
        doStep(boxes, label, operation[0], focalLength);
    }
    return totalFocusingPower(boxes);
}

const std::string& sampleInput = "rn=1,cm-,qp=3,cm=2,qp-,pc=4,ot=9,ab=5,pc-,pc=6,ot=7";
TEST(Aoc15Tests, testtest) {
    ASSERT_EQ(1,1);
}

TEST(Aoc15Tests, sample_doTheThing_1320) {
    ASSERT_EQ(1320, doTheThing(sampleInput));
}

TEST(Aoc15Tests, sample_doTheThing2_145) {
    ASSERT_EQ(145, doTheThing2(sampleInput));
}

const std::string puzzleInput = "mxqlrq=1,xm-,tmkk=2,cx-,cjnjk=9,mc-,bkjk-,hg-,bjrh-,pk-,cnqxh=1,kd=9,cxz-,njrs=1,spg=7,jd=3,zqp-,tb-,hbf-,sr=9,tstl=2,svqt=3,pnh-,xntl-,xb-,mqck-,sggv=3,fj=5,glfm-,qpr-,rdf-,xlzx-,rl-,ztq=9,srzrn-,vsvng=6,sz-,rx-,xntl=3,xqghf=8,kjnzk-,fcf-,mx-,gfm-,hdq=5,mhvl=8,chdm=3,jss=2,qd=9,ftrp=2,vgrmf=2,nbq-,tdm-,sk=5,srgg-,smc-,hsdrm=6,lrkxq-,ddv-,bkfs=1,fvmpx=4,rl-,rd=4,pk=3,jj=3,ptmdv=8,qtc-,fzz-,pgz-,txs-,rq=3,sjmsl-,hgt-,bng-,gj=9,gscz-,gcj=7,sk-,mqbh-,hzh-,xvsqld=7,jqhllm-,vbmjh=7,pk=8,kxs-,prlkbr=8,zxb-,gcng=1,sjb=9,btl=9,tt-,vq=5,xf-,pxv=6,spj-,hj-,qt=6,vgrmf-,cxz-,gqs-,xqghf=6,ngmf=6,fznb-,sxp-,rkj=3,cc-,dcp-,qn=7,ngcd-,cfz=7,rkj-,npxbcm-,cf=5,ldbdn=7,cz=1,xjd=2,kn-,mp-,khqn=2,pxv=1,qg-,tfl=8,prlkbr=9,bz-,hsgvhg=7,jss=6,gcj=5,qdll=1,tdj=1,rq-,pg=3,zh-,bkfs=5,frv-,lrm=2,fznb=1,dt=6,stcx=8,msk-,khb=5,dxj-,cnqxh-,qc=2,fqjb=4,mj=5,ddg=1,rnbkt-,bd=6,gcng=1,zgs-,glc=1,ncm-,xsr-,glfm-,hbf-,vxc=5,cs-,kmk-,hzh=9,zxb=7,ngcd=3,mlnjt-,vrr=8,fd=5,qdll-,dtx-,ptmdv-,hrx=6,kc=5,srgg=2,kqvs=1,hphq=7,fvmpx-,mqck-,spj=7,bdbfs-,mf-,glfm-,rnr-,pzdf-,pk-,kqpd-,kn=4,rn-,dcp-,glfm=2,gf=4,tdj=4,nf-,cc=6,cnqxh=7,nx-,fhgn-,vsvng=3,bjrh-,xjfq-,rq=5,ltsk=9,qd=4,rvc-,qq=7,zgk-,fvf-,ngh-,mpm=1,tsq=3,znvb-,chdm=7,gvnrzl-,vtzc-,bz-,smc-,cf=1,tstl=7,lrkxq-,mvv-,tb=6,cv-,xc=1,rd-,qgjrqh=9,mmhzk=4,njrs=1,rs=6,hgt=3,rdhfk=8,fzz=5,sgbb=3,mp-,bz-,tmkk=4,dxj-,hzh-,gg-,vhm-,mj=9,kmk-,prbf-,gnrx=9,mx-,zv=8,bptp-,pmfm-,xrm-,rkj-,gvhpth=3,jb=2,rdf-,vzc=1,gq=4,dk-,mf-,sjb-,fpzdm-,mgv=5,flhg-,fvmpx-,jvln-,ptmdv-,hsdrm-,vgrmf=3,frv-,btl-,dnvlk=2,qkd=4,frv-,rv=7,ztq=5,hks=2,sqh-,hxbq-,mx=3,zth=2,nvx=3,lx=4,sqh-,hxbq=2,zqp=4,vcjfg-,txl=8,fnz=1,gq-,jgpq=6,gsjb-,jss-,zj=2,nxkkp-,tf=8,dtvv-,nvx-,sqqd=3,fnf-,hgj=8,nt=7,qkd=4,dqd=6,bz-,xqzgsj-,hbf-,sk-,ms=4,qpr-,kqpd-,vcjfg=7,qkd-,rn-,pzdf=8,rf-,frv=7,ld=1,ngmf-,vzr-,dqd-,vb=3,xjf-,kdm=8,tt=8,jvhbhb=9,gl-,mpm-,xqzgsj-,tsq=1,kjnzk=9,rtk-,sdxvb-,vkkn=2,vgs-,nx-,lg-,hxbq=5,tstl=1,xl=1,lt=2,xsh-,hzr=1,dk=8,tmkk=6,cg-,mqck-,jcb=3,tdj-,skpfrh=9,qxl-,kv-,hj=9,dh-,hdg=3,dbv=1,tdq=7,tf=5,fdq-,khqn-,kqpd-,kb-,pbjrl=4,tmz-,lnrlg=8,kqpd-,fr-,tqcd=6,sggv=9,xgt=8,gjpjd=3,bfgs-,zv-,kh-,pmfm=9,frr-,sxkhmx=4,txs=2,vsvng=4,lrkxq-,sjh-,vm=7,hks-,xqghf=9,tjh=7,rs=9,dtx-,sjb-,dz=2,bfgs-,qz=9,mnsb-,gq-,xqghf-,prbf-,xqghf=9,vjv-,hks-,hbf-,rlm-,xhk=1,vrr-,rpj=9,flhg=3,mnqhk=6,gf-,dh-,zk=6,xjf-,gsl=2,gqs-,klk-,bxcxbv=4,mlf=4,gglh-,svqt-,rnbkt-,mr-,vmn-,hxbq-,rd-,jmd-,gflf-,sqrhd-,htmqq-,srzrn-,pvb=7,tllp=1,lvm-,qkxb=2,kxs-,pmfm-,gbkh-,pfvvr-,xml=7,xgq-,cc=5,lmh=2,kc=9,hsdrm=5,mpgs-,bkjk=4,vzr=8,xsh-,ncm=8,gqs=4,rdhfk=3,ncm-,tpqx=5,bkjk=2,mbj=3,lvm=8,cf=8,fxvmbm=5,jvln-,jr=5,gx-,mrxpxr=2,zlpc=1,zgk=5,skcp-,fnj=5,lmh-,rnbkt=7,pg-,sqrhd=6,rdf-,zzd-,cf-,ld=4,mf-,sdhx-,mlnjt-,flhg=7,qdll=5,vzr=3,ddg=8,mvv-,rm-,tstl=5,lc=6,ln=3,dtx=8,gvnrzl=7,jd=6,qqh-,kb-,rjv-,nh=2,dqm-,vzc=2,hx-,tjrx=3,dt=8,vs=1,zpbg=6,hphq-,gvnrzl-,mh-,zx-,srhd=6,qt=1,kqpd=9,dbv=1,cqrpv-,cl=1,fzz-,fpzdm-,sjj=4,xntl=9,xqzgsj-,mhvl=7,kqpd-,lmvz-,hq-,kfrj-,xml=8,hfj=3,nx=5,hq=8,sxrr-,lt-,cnqxh=4,hg-,gz-,snl=2,kj=4,xsfx-,dlcq=3,jcb-,pvz-,bkgd=4,rnbkt-,vtzc-,hk-,sr-,rnr-,rvc=3,sjj=6,xm=6,smb=5,gvnrzl=9,rlm=1,pk-,fdq-,qkxb=6,bdbfs-,xgq-,czjj-,skcp=3,cf-,kgv-,fpzdm-,hrc=9,sqrs-,nfh=4,nzp-,qg-,kz=3,dmn=6,mqbh=6,hzr-,nx=2,mf-,dxzv=2,gvnrzl-,gj=9,flhg-,sf-,lmgh-,hphq=5,msh=3,hzh=3,dlcq-,ms=8,zlpc-,bckn=4,ftrp=3,bng-,zbs=1,gq-,lmvz=6,gr-,nxxng=7,fvf=1,hrc=4,cg=1,rfqj-,xs=5,htmqq=4,sqrhd=1,ckvk-,vjrz-,mnsb=1,tf=2,bz=1,zj=3,smb-,nfh=3,xl-,sjh-,qkd-,jvln-,rnr-,nh-,snl-,kff-,nvz-,kqpd=6,czg=2,vn-,pxfqkh-,qpr=8,bk-,rj=4,qtc=9,fnps-,tjh-,jnxl-,sjb-,ngp-,fhn-,sgbb=9,prbf-,bz=7,mf=9,spg=5,dmn-,fj-,bptp=8,tfl=2,khb-,tfl=8,hb=3,cnqxh-,tf-,hmf-,zj-,vt=3,bdbfs-,sqrhd-,sdhx-,sjj-,ngh=1,hx=1,rvc-,qqh-,rdhfk-,kqd-,cnqxh=9,hvm-,flhg-,scnl=1,kvc-,nf-,jd=3,xvsqld-,rn-,cx-,mx=7,tzb=9,kqpd-,vgrmf-,bdbfs=8,cx=8,hb=2,bxr=8,nt-,ssdx-,dz=6,xqzgsj-,hgj=8,kqd=1,qxl=8,spg=4,vn-,vsvng-,mc=1,hzr-,jmd=7,khqn=1,svqt-,bfd-,hmf=5,mlnjt-,lvm-,xml-,tqcd=6,kff=7,rmr-,jj=8,vpmqb-,lt-,mnmkt=3,kqpd=8,fcf-,qh-,bpx-,kvc-,xjf-,pmfm-,nzp-,jkh-,gr-,crzkft-,vklpj=8,vq-,klk-,vcjfg=3,pxfqkh-,qxl-,fhc=7,tpqx-,jsd-,vbmjh-,lrkxq-,hdq-,rlm-,cx=2,tt=2,jj-,hphq=9,cc-,hx=2,dh=1,txs=2,vs=7,vsvng-,nf=8,hks-,br-,fxvmbm-,dxj-,tdj=9,zp-,kd-,fr=2,lnrlg=6,zsvpk=1,rjv=6,mpm-,bmpkj=9,bgzd=3,kqd-,vrr=6,tsq-,fnz-,htmqq=2,zx=8,mnqhk-,pgz=9,zk-,jnxl=6,qd-,sjb-,tjh=5,glf-,bkfs-,qpr=4,bl=4,srzrn=1,bsz-,sjj=8,gss-,kb=4,kjnzk-,hq=5,tzb-,xgq=7,fvf-,hzh=7,hzzgv=1,dmzjr=8,hj=6,gvnrzl-,hzr-,xlzx-,mmhzk-,bfd=4,hgt=4,qgjrqh-,czjj-,dn=2,szz=1,mk-,hz-,khqn=4,vgrmf-,dcp-,gj-,ln=7,xl=5,hzzgv=4,fcf=2,zk=1,ptb=1,dnvlk-,jd-,nbq-,jr-,xs=4,mnl-,xjd=4,zsvpk=8,nfg=9,sxrr=6,fn=1,ngmf-,kg=8,sf=6,vbg-,xs-,gsl=8,hsgvhg-,qpr-,ss=8,bjrh-,xsr=9,qg=8,znvb-,sdhx-,hrx-,nx=8,smb-,zv=9,pql-,xgtl-,dnvlk-,vzc-,kvc=4,fdq=4,sxkhmx-,dmzjr=8,dmzjr-,tjh=6,jvln-,ngcd-,qhx-,hm=7,zn=4,vhx=1,srhd=6,cg-,xl-,bsz=8,rfqj=7,db-,bfgs=6,sxrr=8,qvptp=9,sj-,bgj-,tf=6,rn=9,fvmpx-,zj-,kz-,znvb-,xjd=1,gjpjd=7,fkcp-,fpzdm-,dlcq-,gl-,dxrnzj-,zgk=7,sjmsl=5,zmx-,tllp-,xjfq=8,gqtkss=6,kg-,xsfx-,hrc=1,gvhpth=3,snl-,htmqq=6,tlf-,mlnjt-,fxvmbm=7,mlnjt=4,mqbh-,mh-,txs=1,tzb-,sk=9,lnrlg-,sdhx-,kff-,ss=8,srzrn=6,dz=7,qz-,dnvlk=1,fdq=6,gvhpth-,qc=9,rl-,gqs=3,mf-,kj=4,scnl-,xhk=1,bfd=6,hgt=4,xf-,zjs=4,gflf-,srhd=2,skcp-,xsfx-,hb=2,mk=9,sxp=1,hb=9,zpbg=5,vz=6,fxvmbm-,js=3,lnrlg=9,xjf=1,gnrx=7,ngh-,bng-,zl=8,tjh-,skcp-,nt=4,jf-,fvf=6,dn=2,mrxpxr-,zj-,vpqfj-,xgtl=8,khb=4,mnmkt-,lmgh=5,bx-,rvc=7,cnqxh=4,gnv-,tb=6,qz=2,sk-,pvz=2,xvsqld-,tcn-,klk=3,tstl=2,prd-,rfv=1,jvln-,zs=1,zx=5,tmz=5,bx=1,mnmkt=6,glc=6,kzs-,mn=7,qt=4,crzkft=4,sjh=1,pk=6,pm-,xb=1,hx=8,bfd=6,vxc=2,bckn-,hbf-,dr-,pvb-,xgtl-,gcng-,xc-,kvc-,mgt=3,rfqj=4,mpm=4,qgjrqh-,cfz-,qgjrqh=6,nh=4,pgz-,glfm-,jxv=5,bmpkj=3,pl=4,ckvk=6,mvv-,dhpll-,fpzdm=9,lx-,fzz=7,mp=7,zgk-,mpgs=9,jj-,hks=6,tdq-,vbvdr=4,dlcq-,bmpkj-,zh=9,skcp-,vsvng=8,pxfqkh-,xhk-,qh=3,ngh=4,hz-,dnvlk=6,vbvdr-,gx-,gf-,klk=5,lc-,ddv-,tmz=3,rkj=4,gss-,qg=3,sjmsl=7,sggv=2,qkxb=4,zqp=9,jxvx-,fvf-,fzz-,ddv=5,zxb-,mvv=4,mnsb=3,gg-,ddg=1,kv=2,klk-,kvc=3,spj=1,dxzv-,rtk=2,gqs-,lq-,mlf=7,fxvmbm-,fznb=2,fb=2,sxp=9,jd-,gsl-,gf=9,hxbq-,rn=1,vgrmf-,kqd-,tb-,tk-,vbmjh=8,rl-,vjv=9,vs-,pzdf=3,ms=3,msh-,tzb=2,vmn-,kqd=2,kg=5,fhgn=2,kfrj-,hzr=6,dhpll-,htmqq=3,nxxng=4,jvln-,rdhfk-,lbx=8,kzs-,gg-,qh=5,fznb=7,hj-,bgj-,vgs-,nbv-,hxd-,snl=8,cf=7,tn=2,rtk-,xsr-,mmp=4,vmn=6,hgj-,bng=1,sxrr-,fkcp-,vz=7,cg-,rfv-,hk-,hdq-,pxv-,mvv=8,bgj-,dt-,xr-,gss-,mnqhk=6,mqx=9,bng-,mc=1,mnqhk-,xs=1,zl=6,dbv=9,zl-,mlnjt-,hk-,tk-,bxcxbv-,fvf=2,vpqfj=2,xml=6,npxbcm=6,jnxl-,cx=1,rm=3,gqtkss=2,prd-,dbg=1,xgq=3,khqn=8,vklpj-,ftrp=8,flhg=3,fnps=5,tmz-,gqs-,jd=2,zgs-,ltsk-,fb-,xc-,gf=8,hdg-,vhx-,glf-,dtvv=2,vgrmf-,xkqz=9,vn=5,nb-,kc=6,dtvv=3,jr=4,bk=3,xgt-,mmhzk=1,zzd=8,dqd-,ldbdn=1,ld=5,sdxvb=7,cc=4,hgt-,qgjrqh-,hrx=7,tfl-,mxqlrq=9,fhc=2,tdj=2,dxzv-,zlpc-,rj=2,qc=3,dtx=9,qd=6,kqpd-,sk-,sjj-,pvb=1,mnmkt=9,kv-,bjrh-,nrvz=2,gr-,bfgs=6,xb-,xsfx-,bk-,txl-,dlcq=3,qqh=4,xhk-,mmp=4,vcrbcj-,mbc-,hdq=3,gflf-,qpr-,pxv-,ttzcm-,skb=1,hk-,hgt=1,zpjgp-,vbmjh-,gsjb-,lp=2,hxd=4,gcng-,lb-,dlcq-,qd-,qg-,sqrhd=9,zpbg-,rpj=3,xml-,xgtl-,zh=1,vs=6,fhn-,sdhx=6,rnbkt=1,mqbh=5,qz=4,sf-,xgt=3,dmzjr=1,rmr-,fxpn-,ptb=8,nxxng=6,pc=3,qg=5,bjrh=5,sqrhd-,kfg=7,pk-,rfqj=7,dxrnzj=8,dxj=1,hvm=8,dnvlk=5,tcn=9,jd=3,vhm=1,gbkh-,mgv=3,bsz-,rq-,kqd=2,tsq-,ngcd=5,msq=1,pgz=7,dtvv-,gg=2,hvm-,czg-,vklpj-,vpmqb=9,rvc-,qc=1,qq=6,hgt=8,xqghf=6,kz=9,sk-,bd=5,fzz-,pql-,tlf-,lq=4,mqbh=3,qhx=7,smb-,cx-,srhd-,hbf-,pl=2,qvnj-,vz=4,sdxvb=4,hzzgv=4,stcx=1,gf-,bgl=9,xqzgsj-,prd-,bxcxbv=4,pv=6,sm=3,bkjk-,zbs-,dk-,bz-,tstl=3,lmgh=5,dxzv-,zgpxg=6,kd-,jxvx=6,czg-,srhd=3,jqhllm-,mx=1,hgj=3,crd=4,tq-,kqvs=7,fb-,vcjfg-,czjj=5,tpqx=8,lrkxq=7,txs=2,bdbfs-,kff-,jgpq=8,rnr=4,rnr-,vdk=1,sggv-,ngh=6,lrm=4,lhdd-,sxrr=6,jnxl=7,sk-,srhd-,cnqxh=2,kxs=9,jgpq-,msk=7,xqghf=6,dr=6,rs=9,vtzc=1,hdq=7,ngp=5,vzc-,ncm-,pxv-,vsvng=8,hxbq-,nfh=4,xml-,hmf=1,zv=4,rdhfk=1,vklpj-,gd=1,fpzdm-,fn=3,dvf-,pxv=2,mpgs=4,mxqlrq=7,sdhx=8,bn=2,jvhbhb-,gzct-,vb-,bbdm-,qkd=6,rn-,khqn=4,kzs-,npxbcm-,stcx=7,lq-,fnps=4,rtk=9,fvf-,ncnh=3,sf-,mhvl=5,fvmpx=2,psm=8,dtvv-,rpj=3,xsh=9,tdq-,ftrp=5,qhx-,pql-,qkxb-,gqtkss-,vzc=3,mpgs-,fxvmbm=1,fxvmbm=3,tstl-,kd-,gbkh-,sxrr=4,vzc=5,vbg=7,hzr=7,nbq-,nzp=1,tmkk-,xlzx=4,rtk-,gz=7,pv=3,sdhx=1,fznb-,lx=1,mnqhk-,rf=5,rjv-,hxbq-,kj-,htmqq-,thh-,pgz-,zpjgp=8,rpj-,mk=9,fd-,xc=6,bz=4,rv=8,fkcp-,jd-,fnz=5,jmd-,ngcd-,qgld=9,sxkhmx=7,kxs-,dqd=2,mz=6,dxj-,dcp-,xc-,ftrp-,jd=5,fkcp=7,bxcxbv=1,rtk-,mnl=1,gg-,svqt=6,fkz-,bz=2,ngp=7,sjj-,rx=9,gvhpth-,ckvk=6,mj=7,crd-,tnr=7,glfm-,tn-,rsc-,nf-,rl-,fnj-,vt-,xgtl-,ngp-,tn=8,gr-,msh=5,gnrx=4,kjnzk-,dh-,xbrxpt=1,vm-,klk=5,vzr=7,mgv=4,lp-,zx=6,htmqq-,zh=3,vmn=1,dvf=3,xjf=9,tjh-,bgzd=6,hz-,bkjk-,bl-,rnr-,kzs-,pxv-,rf-,jkh=4,xsr=4,bd=8,fqjb-,vbmjh=8,xs=5,cl=8,pl=7,pql=3,hx-,txl-,kdm-,dxrnzj=2,qvnj=6,nbv-,rdf-,kdm=3,rdf=9,tdm-,ms=2,xbrxpt-,sjh=5,zgs-,gsjb-,dr-,bbdm-,nt-,sm-,qvnj=3,mnqhk=3,fd=1,rnr=7,dnvlk=7,qh-,nzn=7,nfh=9,xf=9,dmn=9,btl-,srhd=6,cf-,qgld=7,jf-,gjpjd=1,fhc-,crd-,pm=9,kh=3,gnrx=9,ddg-,qq-,mgt-,txl=2,ngcd=1,hbf-,xjd-,pk-,hvm=5,bckn=3,qn-,jvhbhb=1,klk-,msk=6,hzr=2,kn-,fkcp=9,rkj=6,hsdrm=3,crd=1,pnh=3,kgv=5,mrxpxr-,zpjgp-,vzc-,xntl=7,vxc=5,kh=4,kjnzk=5,zsvpk-,qq-,gf=2,qhx-,qn=8,gflf-,hb=3,qd=1,qg-,mlnjt-,rq-,mqx-,tt-,sxp=8,fhn-,ngcd=3,nd-,dxj=9,srgg-,bn=2,lmh-,nfh-,vmn-,hvm-,xvsqld=4,jgpq=6,mhvl-,lg=4,dnb=2,gvnrzl=7,sqh-,xm-,fhc-,gsjb=5,sqrs=6,rl-,vz=7,lmh=8,vpmqb-,vbmjh-,xvsqld=2,ptb-,fnps-,gj=5,sjj=4,vklpj-,nfg=9,vt-,xl-,dz=6,pl=1,xm=6,glfm-,vn-,lrm-,vrr-,dnb=4,kfg=2,xm-,lx-,rsc=4,msq=7,xkqz=5,vrr-,zzd-,bz-,mnqhk-,tzb=2,njrs-,br-,sjb=6,stcx=7,sjh-,kfrj-,xgtl=6,stcx-,ld=5,ngcd=1,vzr-,vzc=6,vpmqb=9,vkkn=6,czg=1,jcb=3,gt=6,tk=2,cg=2,rfv=3,js-,zgs-,vt=6,mqx-,bgj=3,xvsqld-,ncm=6,pmfm-,scnl-,prd=9,gqtkss=6,vz=8,ptb-,jgpq-,mqck-,pmfm-,sj=7,nvx-,jvln=1,zth=6,nx-,msh-,hg-,gl-,zk-,prlkbr=2,nxxng-,zgpxg=9,xf=5,ddv=1,hrc-,qtc=4,zs=4,bsz=6,mqbh-,tzb=8,gj-,rn-,zqp-,smb=9,vrr-,zxb=5,hsgvhg=5,xml=4,mqx-,dh=5,mmhzk-,qh=2,qtc-,rsc-,kgv=9,sm-,mr=3,cnqxh=7,vzr-,bckn=5,dnvlk=3,tnr=6,fvmpx-,kjnzk-,fdq=2,rlm=2,hrx-,zk-,jss-,bkjk-,fj-,zxb=1,kd=3,xrm=1,fhc=3,sggv-,zp-,dxzv=1,frkc=1,lb=8,fhgn=1,srzrn=7,lq=8,vzr=5,xc-,tnr-,qn-,kg-,jvhbhb-,ngmf=3,tqcd-,nzp=5,cc=8,zgpxg-,nxxng-,mx-,pk-,gglh=4,rn-,kc=7,xhk-,zj-,fznb=7,mrxpxr-,dbv-,snl=8,xsr-,khqn=7,mnsb-,mpnvk-,bsqt-,sqh=5,mk=1,vbg-,lg-,ngp-,rv=3,qc=4,nrvz=4,kzs-,lc=3,tjh=4,nb=1,rfqj-,gsjb=4,mmp=5,rm-,jgpq-,vkkn=3,srgg-,xjd-,sxkhmx=5,tdq-,ncnh-,cs=7,hfj-,mpnvk=4,fhgn-,prlkbr=6,xqghf=8,vdk-,xs-,js=5,prbf=9,lhdd=3,prd-,kzs-,gx=4,cqrpv=1,mpgs=4,lmgh=7,mqbh=1,nvx-,fn-,dz-,pfvvr=9,pnh=3,vb=5,tmkk-,crd=8,vbvdr-,hmf-,xl-,dnvlk-,bfd-,gvnrzl-,mbc=2,pc=7,cc-,hgt=8,rlm-,kff=9,xqzgsj=5,rv=7,ftrp-,xhk=5,fhgn=1,vq=2,rs=2,kj-,crd=4,qpr-,kdm=4,kvc-,xjd=7,rdf=5,tt=4,njrs=4,dh-,qxl=9,zh-,xvsqld=6,htmqq-,sxp=1,frv-,zx-,glc=4,pql-,dmd=2,ssdx=7,dr-,pvb-,dz-,tmz=7,sj=1,kfrj-,mp=8,skpfrh-,gx-,mqx-,rm=2,xbrxpt=1,rjv=5,pmfm-,mbj=4,vrr-,kzs-,sqqd=4,fhc-,szz-,xqghf=3,jd=2,vsvng-,xm-,qpr-,dbg=6,bbdm-,gg=5,sdxvb=7,pgz=5,lmgh-,mgv=7,tdm=3,bkfs=1,gvnrzl=5,mz=1,dqd-,xb-,xvsqld=3,fc-,vcrbcj=8,dvf=8,bbdm-,frv=2,vdk-,zpbg-,kb-,dtvv=9,qvptp-,sbm-,sqh-,nzn=5,fdq-,nx=3,tqcd=1,rjv-,qz=7,jj-,mlf-,bmpkj=5,tllp=2,rv=6,mnl-,nbq-,mj=4,gvnrzl=6,bng-,gd-,vgs=1,nfh=1,rnr-,mj=5,zsvpk=7,pnh-,qxl-,chdm=1,zp=9,fkcp-,pm-,fr-,sqrs-,gc-,bz-,cl=7,dqm-,sdhx-,zbs=8,jxv=4,rsc-,zh=7,fxpn-,dl-,bgj-,gss=4,xl-,xc-,xl-,gnv-,jsd-,xntl-,ptb=5,skpfrh-,hdq=1,zjn-,msk=4,kzs-,bpx-,hq=5,srgg=7,hns=1,znvb-,hxd=4,jqhllm-,bng-,fj=5,tmkk-,gckjpg-,srzrn-,sjmsl-,jj-,qn-,tmz-,js=4,gcng-,cg-,spj-,xntl=9,nvx-,skkg-,bpx-,glfm-,zgs=7,tk=7,hg-,gflf=5,zj=9,gscz-,lnrlg-,txl-,spj=6,rl-,bptp=8,tq=7,lg=7,mlf=3,gl-,mqck=7,sjh=7,qq-,zv=8,rdf-,zjs=4,zxb=1,rfv=6,rd=3,hg-,nh-,jxvx=2,dn-,gckjpg-,gcj=6,rkz-,hxd-,gglh=9,pmfm=3,lhdd=2,ldbdn=2,zbs-,frv=7,kz-,fb-,clt=7,skcp-,gd-,fkcp=5,jf=8,tjh=4,bsz-,skb=6,xm=9,rdf-,fhc-,nx-,fr=8,rf=6,vxc=3,tzb-,jr=4,hk-,qgld=7,jf-,gvhpth=8,vrr=5,mh=7,zs=4,zgs=9,hks=1,mj-,dnvlk=2,sggv=4,kdm-,zgpxg-,rj=6,"
"fznb-,pl-,xgt=9,gflf-,pg=7,tb=5,sqrhd=3,mbj=7,zj-,sqh-,qvptp=3,vbmjh-,dn=5,mnqhk=6,pv=6,bsqt-,zsvpk=7,sm=6,lb=4,sj-,zbs=3,pxv-,dqm-,vkkn-,rkj-,qgjrqh-,fhc-,kh=9,pfvvr=4,rdf=2,jss-,dvf=6,mhvl-,kz=3,lmgh=1,xgtl=9,zbs=1,rq=1,kqvs=5,fhc-,bckn=2,msh-,hb=9,tllp-,htmqq-,tdm-,mbj=3,ln-,hz=4,kdm-,pc=5,crzkft-,qdll-,dqm=8,tcn-,ncnh-,skpfrh=6,rsc-,mlnjt-,zpjgp-,kmk-,rf=6,fd=3,chdm=7,xm-,kdm=8,zr-,xb-,nbv-,gsjb-,ngp=8,lnrlg-,mpnvk-,hdg-,pm-,sjj-,tdj=3,mrxpxr-,msh-,smc=7,vklpj-,sggv-,xgtl=8,mr=6,xhk-,pl-,tn=4,xf=1,vn-,cfz-,dn=6,gcng-,zsvpk-,qqh-,kdm-,hx-,czjj-,cz=6,qgld=9,gsl=7,fnz=4,qzk=3,hrc-,rkz=7,gx=4,nh=8,tq-,zl-,nt-,kfg-,xjf-,qzrd-,gnv-,lq=8,nrvz-,fzz=9,dk=1,mmhzk-,dmzjr=4,mrxpxr=4,zs-,jgpq=2,mf-,xbrxpt=9,txl-,bn-,vbmjh-,gqtkss-,prd=3,jqhllm=1,gfm-,xm=1,hsdrm=6,dhpll=5,gvhpth=8,js-,ms=8,qkxb=4,lmh=4,szz-,mj-,qgjrqh=4,ddv=1,mn=3,mvv=5,tlf=8,pxfqkh-,hfj=8,qvn-,gq=8,ckvk=5,fznb-,kxs-,jvln=5,lrkxq=4,xs-,hzh-,lmh-,tmkk-,bmpkj=7,xntl=5,lq-,ngp=6,qn=1,jvln=1,zsvpk=3,kd=5,kn=3,hks-,gfm=5,dbv-,kz-,mvv=5,rl-,gc-,tsq-,xjfq=5,mk-,xf=1,dmn-,smc=9,qc-,xqzgsj-,kfrj-,bpx=9,dlcq=9,qqh=9,vz=4,gqtkss-,vxc=8,frv-,pql=5,zj=4,qvptp-,hsdrm=6,rf=4,pgz=8,bjrh=4,hrx=9,hdg=6,kqpd-,kj-,tpqx=3,tzb-,dcp-,nbv=4,xjfq-,khb-,jgpq-,gj=7,bxcxbv=9,rkz=5,smc=8,fznb=6,rlm=3,mj-,msq-,pvb-,kjnzk=9,vsvng=7,nh-,bkgd-,bd=5,fj-,hxd-,rj=8,frr=8,nf-,tk-,zv=1,fkmhdn=8,lrkxq=6,btl-,gflf-,vjrz=1,dmzjr-,htmqq=4,kc=8,hsdrm-,zl-,bz-,qxl=4,kvc-,fnz-,bgzd-,kn=4,tt=3,sggv=8,mgt-,tlf-,xbrxpt=9,sxkhmx=5,qxl=6,bkgd-,rj=9,flhg=8,tdm=4,fpzdm-,gsjb=3,crd-,cs-,cg=5,bfgs=9,bdbfs=7,tcn-,xml=3,mnsb=8,pzdf-,rd=9,fkz=8,fxb=3,ftmfz-,sdxvb-,qvptp-,sxkhmx-,nfh=8,lnrlg=8,fxpn-,ccd-,tcn-,lmvz-,ms=8,ptb-,skb-,qhx=6,xkqz-,vm=1,msk-,hzr=2,lc-,vdk-,cjnjk=5,cnqxh=8,vt=8,prd-,zjs-,gf=6,mmp=8,gss-,xrm=4,bhfnj-,jcb=7,pmfm-,fhc-,vtzc=5,gglh=4,spj-,hdq=9,bgj=8,rpj=2,gsl=1,tllp-,mqx-,hzh=4,pbjrl-,dxj-,tstl-,vzr-,htmqq=1,zn=7,db-,fhc-,cxz=2,nzn-,vmn-,fzz-,tjh-,kqvs=3,rfqj-,lq=1,ptmdv-,xf-,vb=8,dk-,dz-,dnb-,hsdrm=9,nf=8,qz=6,lb=9,mh-,xb=2,hm-,svqt=3,ptb-,frr-,jj-,mnsb-,gvnrzl-,vjrz=6,rv-,gzct-,rnbkt=9,xntl=3,dbv=1,kgv=8,bsqt=7,dh-,pfvvr=4,mnsb=8,hphq=5,hzzgv-,qzk=6,gvnrzl-,fnz=2,xsr=9,pql-,zjs=3,crzkft=2,jvln=6,vm=3,hsdrm-,qdll=5,glf=9,qvptp-,nfh=5,gc=3,kqvs=4,rx=6,dvf=3,gcj=4,mk=7,fr-,jkh=4,dxrnzj=7,tstl=4,gnv-,vhm-,spj-,xlzx=2,ccd=4,fj=5,lmvz-,bgj=2,xlzx=6,tqcd-,xjf-,znvb=5,vzc-,prlkbr=7,lq-,xrm-,xf-,pc-,mr=8,fkcp=3,qxl-,bfgs-,mlnjt-,hx=5,nbq-,bpx=4,glc=2,glc-,lg=3,rkj-,cqrpv-,gqtkss=6,stcx-,bxcxbv-,pxv-,nrvz=9,frv=1,xjfq=1,cx=1,prlkbr=5,vjv=3,flhg-,qdll-,zbs-,sjh=3,sdhx=9,gnv=1,tlf=7,qvn=4,gx-,ckvk-,mc=4,fqjb=8,mh=7,cqrpv=4,tnr-,bfgs=2,mxqlrq-,lp-,sf-,vq-,skpfrh-,zs=8,sgbb=9,dxrnzj=4,ttzcm-,fhn-,psm=5,nd=7,skb-,vrr-,spj-,jsd=3,dh=6,mj-,mvv=3,xbrxpt-,zjn=2,tdj=1,gnv-,fkz=5,gglh=7,gj=2,jkh-,dl=8,msh-,jxv-,tsq=4,hks-,sxrr=3,nzp=9,bgl=3,skpfrh=5,qvn=2,zl-,prd=5,vm=8,bl=1,fvmpx-,ms-,zn=6,mnsb=4,kn-,dbv=6,mnqhk-,mnmkt-,tqcd=4,ncm=6,khb-,gsjb=3,rnbkt-,cc-,mxqlrq-,br-,jxvx=9,hk=2,gc=7,nbq=5,ss-,hdg-,xhk-,cc=1,vjrz-,qz-,lvm-,hgt-,jb-,hx-,vbmjh-,dl=7,bk-,prlkbr=7,zk-,nxkkp=9,lq=3,flhg-,hfj-,xc-,nfg-,pbjrl-,fnf-,bgl=6,bk-,mgv-,gqs=5,sdhx-,hzzgv-,hns-,gsjb-,pfvvr=9,crzkft-,bk-,rvc=8,gvhpth=1,rv=7,mnmkt=3,kzs-,gcng-,mlnjt-,zgs-,sqh=9,jss=2,dvf-,nd=8,fhc=3,gd=7,glfm=3,tdm=5,fnps-,jgpq=4,vklpj=6,bng-,jb=8,svqt-,mgv=4,rl=3,xjd-,zr=3,hgj-,bfd-,jvhbhb=6,dbv-,vtzc=1,pxfqkh-,zp-,zn=3,qdll=5,gz-,qgld=9,kfrj-,xsfx-,sjh=7,nx-,kqvs=5,khqn-,mh=4,mp-,vkkn=3,tdj-,rdhfk-,ptb=2,xl-,gnrx-,bfgs=8,npxbcm-,lvm=4,kmk-,lp-,rfv=7,mk=5,xsr=6,hj=4,plqdzc=4,sz=1,nd-,cl=8,gz-,pmfm=7,fqjb-,hzzgv=6,lb=6,nrvz=9,tdj-,zp-,lp-,sj-,mpnvk-,mbc-,rq=7,dn-,kv=7,lt=9,srhd-,xjfq-,kv=8,nxxng=8,sdhx-,bn-,vsvng-,tstl=6,dtvv=8,xf=9,qzrd=4,fqjb=5,lrkxq=7,xml=1,vpmqb=3,lrm=4,db=1,jb-,gd-,tzb=4,kv-,jqhllm-,qvn-,hdg-,dcp-,gg-,sjb-,kqvs-,ptb-,qxl=5,vbmjh-,sqh=1,tk-,jd-,zgpxg=5,bn-,vz=3,pzdf=2,lp-,mlnjt=3,sf=7,qc=3,qn=6,zv=9,tmz=6,hbf-,zlpc-,bgl=2,bd-,sm-,sr-,skpfrh-,fvmpx-,xgtl=4,zgpxg-,fcf=8,qn=3,jgpq=5,kjnzk-,dhpll-,fhgn-,xml=4,tt=7,hbf-,zqp=5,fnps-,srzrn=3,zsvpk=8,nfh=8,ftmfz=7,sxkhmx=8,bkjk-,vt=6,dh-,ckvk=9,rfv=8,ln-,rlm-,dk=4,sjh-,pl-,mlf=6,vtzc-,nfh-,vcjfg-,smc-,jqhllm-,rm-,cl=1,dmn-,sbm=1,kzs-,kgv-,mj-,ngmf-,nzn=2,lt-,msh=9,kvc-,hdq=4,hz-,dz-,ltsk=2,jsd-,fzz-,qgld=7,hzr=3,gqtkss-,vbvdr-,gcng=8,nh=7,njrs=5,glfm-,qzrd-,lp=8,fkz-,fn-,ngh=2,cfz-,vpqfj-,ss=1,cz=3,xl-,lb-,vjv-,mnsb-,ln=8,dxj-,bfd=3,bxcxbv-,mqck=4,ss-,gvhpth=6,rm-,rdhfk-,npxbcm-,xjd-,ssdx-,fxpn=9,tzb-,cf-,vcjfg=7,kd-,sqh-,dlcq=3,dbv-,dk=1,vs-,jxv=6,prd=9,lp=7,pg=1,lbvt-,qxl-,crzkft-,vn=6,srhd=3,nb-,kv=5,bpx-,fznb-,hrc-,flhg-,bckn-,hdq-,prlkbr-,cv=1,gfm-,gzct-,hzzgv=9,pg=4,tnr-,fcf-,frv-,hj=1,jnxl=8,gsjb-,hfj-,nxkkp-,vpmqb-,sr=9,qvptp=1,rnbkt=2,vxc=4,srhd=5,lb=4,vcjfg=7,rsc-,vjrz-,gr-,lb-,kd=2,dmn=9,bng=8,qtc-,dxj=3,tcn-,dhpll-,gqs=7,nx=6,xm-,bsz-,bkfs=7,sggv=4,tq=6,rf-,hzzgv=1,ss-,msk=3,vs=5,fxb=1,qkxb-,hm-,qgld-,dxj-,xjd-,qz-,txs=2,zjn-,rkz=6,vjv=4,glf=2,flhg-,rd=4,rnr=3,vpqfj-,thh=5,mqck=5,qzrd=9,znvb=8,kfg-,mhvl-,nxkkp-,gl=4,fnps-,stcx-,lp-,dhpll=2,pl=9,mxqlrq-,nb-,kqd-,jxvx=4,bk=7,qxl-,qz=2,qxl-,sqrhd-,gss-,vzc-,vz-,psm-,jf-,hmf-,dvf=4,jcb-,msk-,sz=8,nvz=1,sxrr=9,tcn=9,bgl=9,pxv-,jd-,vbmjh=9,dl=1,msk-,cqrpv-,ckvk-,ngp=6,zp-,mf-,rkj-,msq-,qtc-,czjj-,kn=8,bfgs=1,hq-,qd=5,qg-,vxc=8,kv-,qkd-,mgt=5,tdj-,xhk=8,hmf=8,vzc-,hk=3,hx-,czjj-,sf-,lmvz-,mqbh-,qzk-,hzr-,mqx=3,kc-,ld-,btl=4,vxc=2,bk=8,mf-,kff-,xr=7,dnvlk-,gcng=2,plqdzc=8,dhpll=4,lmgh=5,zj-,rj=8,pg=3,br=9,srzrn-,prlkbr=1,xgtl-,dxj=2,tt-,dbv=2,hj-,jqhllm=6,tllp-,xjd-,ln-,mhvl-,rnbkt=8,fc=8,sjb=5,zjn=8,fj=2,kmk-,htmqq=1,ftmfz-,mhvl-,sdxvb-,cg=3,qkxb-,gcng-,gss=6,vs=1,kzs=2,ddv-,fb=8,mgt=7,jd=7,hzr=3,dtvv-,vz-,qh=8,rkz=1,pg=2,mbj=5,sdxvb-,srzrn=4,hbf=3,zv-,vq-,rmr=5,hxd-,qd=2,dz-,plqdzc=1,jr=1,ttzcm=6,prbf=4,tstl-,vmn=9,btl=7,hk-,gsjb=9,jss=9,mrxpxr=3,bgzd=5,nh-,lhdd-,hg=6,skb=4,pzdf-,bbdm-,sm=6,mmp-,xjfq-,kjnzk-,kdm=5,pxv-,jnxl=2,qvptp=1,ln-,vs=9,msk-,pmfm=6,fhc-,nx-,tt=4,tllp=4,dmn-,xvsqld=9,vtzc-,sr-,kv=2,bxcxbv-,rmr=5,pql=3,pc=3,kb=8,zjs=8,zn=4,rq-,sqrhd-,sf-,ldbdn-,br=9,xhk-,ltsk=9,cx-,gflf=5,pmfm-,zgpxg=7,fkz-,xjfq=9,hgj-,bsqt=1,nh-,cv-,fkmhdn-,zv=9,mgt=4,gss=9,kj=5,sbm=6,sm-,gjpjd=3,kfrj=2,qz-,tfl=7,mlnjt-,scnl=5,mnl-,lt-,rnbkt=9,xbrxpt=1,rl=4,chdm=5,njrs-,gqs=9,zpjgp-,pzdf-,bsz-,xgt-,fb=8,gq-,sz=6,bng-,vgs=3,ssdx-,qgjrqh=9,lq=7,fpzdm-,cv-,sjh=5,zsvpk=8,vtzc=7,lq-,hsgvhg-,pxfqkh=2,mh=7,ftmfz-,fkcp=6,jf-,khqn-,rkj=8,gg-,fkmhdn=6,rx=3,sz=6,vklpj=1,npxbcm-,hrx=4,vmn=9,sqrhd-,mqbh=6,hsdrm=5,sqqd=3,pv-,qzrd=1,gglh-,xl=4,dh-,tsq=5,tstl=7,xjd-,pl-,zpjgp-,vcjfg-,rlm-,kn=8,fnj=5,zn-,mr-,hk-,bl-,ldbdn=5,zpbg-,crd-,ncnh-,vzc-,zth=5,bd-,zs=9,fvf-,hxbq=2,nt-,cv=6,gzct-,jb=4,vcjfg=6,hrx=3,gd=7,tzb-,vbmjh-,pbjrl-,gckjpg-,fhn-,sjmsl-,rmr-,htmqq-,hg=3,vbg=4,br=7,vhm=6,kh-,smc=4,nxkkp=1,kb-,crd-,gscz-,gflf=3,rf-,mpnvk-,srhd=7,czg-,bn-,nrvz=9,spj-,lt-,bng=2,vgs-,glc=9,gzct-,fc=1,vs-,hphq-,tjh=4,lx-,kqd=3,glf=2,lrkxq=1,sr-,mvv=2,ltsk-,rmr=4,dvf=6,sm-,dh-,qvnj-,mk-,szz-,psm-,qdll=1,szz=5,gnrx-,gjpjd-,pfvvr-,lmgh-,vpmqb-,frkc=3,btl=8,nzp=4,tfl=7,xl=4,qt=9,xl-,nzp-,khb=6,hg=5,rvc-,kjnzk=6,pvz-,nt-,xsh-,zqp-,rq=5,fc-,xjfq-,pm=8,sgbb-,qgjrqh=2,bx-,htmqq=6,hrx-,db-,kfrj-,zxb-,rl=7,mlf=2,kb=5,fhc-,kg-,hbf-,jb-,prlkbr-,mgv=9,qkxb=2,pm-,jr=6,dl=5,gfm=7,rkz=5,mj-,xntl=7,spg=5,rvc=3,kqd=9,xjfq-,thh=2,ncnh=2,vjv-,vcjfg-,vxc=9,dtx=9,zsvpk=2,dqm=7,gc-,htmqq-,dn=8,sjb=2,tlf-,txl-,nx-,dn=9,ngmf=2,jj=3,lvm=4,jgpq-,mbc-,mnl-,kn-,hzh=6,pc=3,sjj-,zh=1,xrm-,tq=3,zbs=9,rmr-,gflf-,zr-,dtvv=1,fhgn-,xkqz=7,vdk-,mqx-,hzzgv=9,rs-,frv-,dbg-,tdj=6,rfqj-,tq-,mhvl-,bfgs=9,kh-,dxj=4,vgrmf-,bdbfs=9,kdm=8,hgt=9,pbjrl-,cxz-,kqvs=9,ckvk-,fvmpx=2,gf=8,fr-,gnv-,cqrpv-,lnrlg=9,pnh-,gsjb-,fzz-,nbq-,hgt-,mmp-,kmk=8,mrxpxr-,sqrhd=9,rkj=2,svqt-,zqp-,pk=3,xqzgsj=4,mvv-,nx=6,rdhfk-,pql-,pfvvr-,qkxb=2,rnr=2,kjnzk=1,pmfm=6,tsq-,snl=7,lt-,nvz-,gflf=6,stcx-,ngh-,gr-,rpj=2,jqhllm-,xs-,vq-,zpbg=5,xqghf-,bk=6,nrvz-,sdhx=9,rn-,nb=9,sdhx=1,tnr-,jcb=4,js=5,tjh=6,ss=7,mqck=7,mj=8,rfqj=3,dcp-,pbjrl=3,pbjrl=4,vjv=8,tpqx-,scnl-,pv-,tsq=1,vzr=1,glf=8,frv=2,btl=8,mgt-,ltsk=2,fkz-,qgld=8,hxd-,vjv-,zpjgp=1,hxbq-,pfvvr=6,tfl-,mqck=4,ngcd=1,fkcp-,rdf=3,bjrh=9,gss=9,frkc-,tn-,hxd=8,xl-,hdq=6,dqd-,pgz=5,fn=6,zbs-,jb=9,jss=7,sf=6,zjs-,sxp=8,rtk=2,sj=6,hm-,zxb=3,rfqj=8,gl=1,qkd=5,qtc=9,xl=8,pc=6,rdf=9,hj=5,xlzx=2,xsh=7,nt=5,vm=9,sjmsl=3,tk=6,zr=6,hdq=2,zh=3,gnv-,ftmfz=3,tjrx=5,ddv-,vxc-,vgs-,ngh=3,lmvz=2,kz-,fdq-,vm-,bpx=4,hrc-,pl=9,tmz=1,kmk-,tq-,bx=6,vpmqb=2,npxbcm=6,cv-,gglh=7,dxzv=8,clt-,mh=9,ks-,mx=2,gqtkss=9,dnvlk-,kjnzk=8,nvx=3,vbg-,vm-,pl-,tpqx=2,mnqhk=5,gfm-,kmk-,vhm-,tjh=3,gcj=7,vcrbcj=8,bmpkj=9,fdq-,xbrxpt-,cfz-,mqbh=3,txl-,rd=3,xbrxpt-,rs=1,ngcd-,fkz-,bk=4,mpnvk-,spj-,mp=4,lmgh-,qtc=7,gqs-,hdq=2,gss-,gj-,bng=4,tlf=7,mqx-,vgrmf-,hxbq-,mh=3,fvmpx-,hdg=1,bfd-,mgv-,hsgvhg=5,dnb=7,qvnj-,qh-,kjnzk=6,qz=6,ncnh=7,fvmpx=6,ngh=6,pl-,sjb=9,qhx-,qq=5,gglh-,rlm-,tfl=8,bbdm-,kxs=6,xqghf-,cl-,mmhzk-,kxs=6,nrvz=6,fc=6,vmn=5,nx-,nvz-,hxd-,ncnh-,xc-,ftmfz-,pzdf=9,srgg-,jf-,tdq-,zl-,zp-,lmvz=7,srhd=7,fznb-,qhx=9,ptmdv-,fc-,dl=3,xbrxpt=6,rnbkt=8,xsr=9,mh=6,xml-,cl=5,xjfq=5,kmk=6,tnr-,bxcxbv=8,db=8,frv-,kdm=7,tmkk=4,dxzv=8,xsh=5,ks=3,jqhllm-,sxrr=3,bhfnj-,kqpd-,lb=9,dr=4,pzdf=5,fqjb-,vt-,rs=9,kh=3,mrxpxr=1,ftrp-,glfm-,xgtl=8,hzh-,spj-,bhfnj-,qzrd=7,kjnzk-,cl-,qn=2,vxc=1,vb-,jf-,nbq-,zth=4,bfgs=2,mqbh=6,hzh=2,kgv-,dn=4,znvb=7,fzz-,xqzgsj-,vbg-,vcjfg-,nfg=3,dtvv-,nxxng=2,fhn-,lt-,gss=3,crd=6,srhd-,kgv=3,rvc=6,sjh-,gqtkss=3,pg=7,npxbcm=8,xsr-,bl-,mn-,nx-,vxc-,lmgh-,sxkhmx=1,mn-,pzdf=4,vklpj-,zgpxg=4,zsvpk=3,hgt=6,lnrlg=2,tk-,gsjb=6,qvnj-,lmvz=7,rm-,bbdm=6,spg-,gz-,lx=2,jd=8,gc=7,hmf-,rvc=6,khb=5,pm=1,gr=9,dl-,zgs=6,thh-,gx=7,hxbq=2,tdj=7,gsl=8,hsgvhg=9,fznb=2,sj-,xb=8,lmgh-,xgtl-,qpr=4,zr-,rm=7,crd=8,vjrz=2,qqh-,msk=2,xsfx-,hrc=3,srhd=7,sdxvb-,hsgvhg=5,gscz-,ddg-,czg=8,zpbg-,qkxb=4,rpj=6,mnmkt-,rl-,ln=7,ln=3,hns-,rdhfk-,skkg=7,ld=1,dbv-,rnr=4,qc-,hvm-,rvc-,msq=3,psm=1,bd=4,bxcxbv-,sjmsl-,gckjpg=3,bkgd-,pgz=1,nzn=1,jf=8,ssdx=6,gd-,jr=8,jkh=5,gjpjd-,mvv-,qtc-,rtk-,fpzdm=3,vhx=2,cfz=6,chdm-,ms=6,sz-,sdxvb=7,hbf-,gflf=4,sggv=3,kb=7,skcp=3,mpnvk-,qt=6,ms-,lp=6,bz=6,hg-,gcng=2,bmpkj=5,jb-,gjpjd=1,mrxpxr=2,pxv-,gsjb-,ncnh-,jf=4,vz=6,msq=2,ld-,pgz-,jr-,tqcd=5,pxv=5,dqm=8,bd-,tqcd-,vzr=6,mj=7,fvmpx-,sqh=3,mqx=4,jmd-,mbc-,vpmqb-,mp=6,qkxb-,sqqd-,nbv-,br-,mnqhk-,qdll-,cc-,qn=4,mj-,zmx=7,nd=1,rd=4,xlzx=5,dqm=9,gl=7,glfm=2,xhk=4,jkh-,bfd-,vkkn=2,qtc-,kd=7,ptb=8,tzb-,tdm=3,xf=5,fc-,nrvz=8,kb-,mhvl-,skpfrh=9,kj=1,ssdx-,hbf=1,qhx-,qvn=2,zbs=4,dbg-,bbdm=1,vzc-,srzrn=5,rjv=9,vt-,jd-,kjnzk=7,kv-,tllp=7,gnv-,fj=4,zn=2,gz=8,zgs-,jgpq=4,pbjrl=9,ftrp=3,dn-,bpx=7,xb=5,mlf=9,fxb-,cl-,dxzv-,ssdx=9,hmf=4,jvhbhb-,jf=9,nbv=8,rlm-,mh=1,tf-,nzn=8,flhg=8,nb-,ptb-,fznb-,gvnrzl-,lrm=3,mxqlrq-,vjv=6,bd-,bjrh-,xf-,jgpq=9,txs=3,dn-,gflf=9,fkcp=3,bdbfs-,mlnjt=5,qvnj=9,xc-,sqrs=9,mqck-,nzp-,xhk-,ftrp=3,fxvmbm=8,pvz=7,prd-,htmqq-,tt=9,xb=6,rv=5,qgjrqh-,fznb-,gbkh=6,tqcd-,qg=2,hsdrm=7,gsl-,dqd=3,vbvdr-,rd-,nbv-,njrs=4,dhpll-,gfm=8,ckvk=5,tmkk-,tdq=5,mh-,fn=7,kv=6,kqd-,pvz=7,bxr-,zpjgp=2,vpmqb-,cv=2,vcjfg-,dn-,hz=7,hns=7,tllp-,ssdx=7,fvf-,jcb-,mn-,nxxng=6,cjnjk=2,xntl-,ngp=1,vjrz-,qkxb=4,js=8,tpqx=6,gckjpg-,rsc-,jf-,db-,skcp=4,zmx-,vq=9,dlcq-,nvz-,kvc-,ngmf=8,gnrx-,hks-,dxj=8,bd-,jxvx-,sjj-,prd=7,gzct-,cv-,lp-,mnqhk=8,xml-,kzs=2,fnf=7,mnmkt-,clt=7,bl-,kvc=1,kzs-,nzn=1,kz=6,dxj-,xkqz-,fdq=4,kv=3,fj=5,jxv-,vxc=7,vsvng=1,ptb-,stcx=8,sjj-,kn-,bkfs-,zgs=6,prlkbr-,msk-,sqrhd=9,kz=3,sqrhd-,qgld-,vhm=9,rmr=8,msh=8,bz=8,lrkxq-";

TEST(Aoc15Tests, puzzle_doTheThing) {
    ASSERT_EQ(0, doTheThing(puzzleInput));
}

// 507769 is too high
TEST(Aoc15Tests, puzzle_doTheThing2) {
    ASSERT_EQ(0, doTheThing2(puzzleInput));
}