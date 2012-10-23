// \file  VectorToc.cpp

#include <stdexcept>

#include "VectorToc.hpp"

using namespace general_processing;

class VectorValueInfo::DeleteVisitor : public VectorTocVisitor {
protected:
    void visit(VectorValueInfo const& info) {
        VectorTocVisitor::visit(info);
        if ( info.content ) {
            delete info.content;
            info.content = 0;
        }
    }
public:
    DeleteVisitor() : VectorTocVisitor(-1) {}
    void apply(VectorToc const& toc) { visit(toc) };
};  // class DeleteVisitor

VectorValueInfo::VectorValueInfo() : 
    placeDescription(""), position(0), castFun(0), content(0) {}

VectorValueInfo::~VectorValueInfo() {
    if (content) {
        DeleteVisitor dv;
        dv.apply(*content);
    }
}

bool VectorValueInfo::operator==(const VectorValueInfo& other ) const {
    return placeDescription == other.placeDescription &&
        position == other.position &&
        castFun == other.castFun;
}


/** Determines whether two tocs are equal or not. */
class VectorToc::EqualityVisitor : public VectorTocVisitor {
    bool mEqual;
    const VectorToc& mThisToc;
    std::vector<const VectorToc&> mOtherStack;
    const VectorValueInfo& mOtherInfo;

protected:
    void visit(VectorValueInfo const& info) {
        if ( !(mOtherInfo == info) ) {
            mEqual = false;
            return;
        }

        if ( mOtherInfo.content > 0 && info.content > 0 ) {
            mOtherStack.push_back(*(mOtherInfo.content));
            VectorTocVisitor::visit(info);
            mOtherStack.pop_back();
        }
        else if ( info.content == 0 && info.content != mOtherInfo.content ) {
            mEqual = false;
            return;
        }
    }

    void visit(VectorToc const& toc) {
        if ( toc.mType != mOtherStack.back().mType || 
             toc.mSlice != mOtherStack.back().mSlice ||
             toc.size() != mOtherStack.back().size() ) {
            
            mEqual = false;
            return;
        }

        VectorToc::const_iterator it = toc.begin(); oit = mOtherStack.back().begin();
        for ( ; it != toc.end(); it++, oit++ ) {
            mOtherInfo = *oit;
            visit(*it);
            if (!mEqual) break;
        }
    }

public:
    EqualityVisitor(const VectorToc& this_toc, int max_depth=-1) : 
        VectorTocVisitor(maxDepth), mThisToc(this_toc) {}

    bool apply(const VectorToc& other) {
        mEqual = true;
        mOtherStack.clear();
        mOtherStack.push_pack(other);
        visit(mThisToc);
        return mEqual;
    }

}; // class EqualityVisitor


VectorToc::VectorToc() : mType(""), mSlice(""), maxDepth(100) {}

std::vector<double> VectorToc::valueToVector (const Typelib::Value& value) {
    std::vector<double> v;
    return v;
}

Eigen::VectorXd VectorToc::valueToEigen (const Typelib::Value& value) {
    std::vector<double> v = valueToVector(value);
    Eigen::VectorXd eigen_vector = Eigen::Map<Eigen::VectorXd>(&(v[0]), v.size());
    return eigen_vector;
}

Typelib::Value VectorToc::vectorToValue (const std::vector<double>& vector) {
    return Typelib::Value();
}

Typelib::Value VectorToc::eigenToValue (const Eigen::VectorXd& vector) {
    std::vector<double> v;
    v.resize(vector.rows(), 0.0);
    Eigen::Map<Eigen::VectorXd> m(&(v[0]),vector.rows());
    m = vector;
    return vectorToValue(v);
}

VectorToc VectorToc::concreteToc (const Typelib::Value& value) {
    return *this;
}

VectorToc VectorToc::slice (const std::string& slice) {
    return *this;
}

bool VectorToc::operator== (const VectorToc& other) {
    EqualityVisitor ev(*this, maxDepth);
    return ev.apply(other);
}

std::vector<std::string> VectorToc::getPlainToc() const {
    static int depth = 0;
    static std::vector<std::string> type_stack;
    static std::
}


void VectorTocVisitor::visit(VectorValueInfo const& info) {
    if (info.content) {
        mDepth++;
        if (mMaxDepth >= 0 && mDepth > mMaxDepth ) 
            throw std::runtime_error("reached maximum recursion depth in VectorTocVistor");
        visit(*(info.content));
        mDepth--;
}

void VectorTocVisitor::visit(VectorToc const& toc) {
    VectorToc::iterator it = toc.begin();
    for ( ; it<toc.end(); it++)
        visit(*it);
}



