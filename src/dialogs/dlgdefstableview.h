#ifndef DLGDEFSTABLEVIEW_H
#define DLGDEFSTABLEVIEW_H

#include <QDialog>
#include <vector>
#include <memory>

#include "../common.h"
#include "../NFSBundle.h"

namespace Ui {
    class DlgDefsTableView;
}

class DlgDefsTableView : public QDialog
{
    Q_OBJECT

public:
    explicit DlgDefsTableView(QWidget *parent = nullptr);
    ~DlgDefsTableView() override;

    void SetBundleAndIdx(CNFSBundle* bundle, int idx);

private:
    using TEntriesVec = std::vector<S_NFSDefsTableEntry>;

private:
    std::unique_ptr<Ui::DlgDefsTableView> ui;
    CNFSBundle*             m_Bundle = nullptr;
    int                     m_Idx    = -1;
    S_NFSDefsTableHeader    m_DefsTableHeader{};
    TEntriesVec             m_DefsTableEntries;
};

#endif // DLGDEFSTABLEVIEW_H