#include "dialogabout.h"
#include "ui_dialogabout.h"

DialogAbout::DialogAbout(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogAbout)
{
    ui->setupUi(this);
    QString msg =
    "This is the SimCenter Educational Pile Group Tool Version 1.0.\n"
    "\n"
    "Please be aware that this software is not intended to be used in any commercial analysis and/or design. "
    "All contributors of NHERI SimCenter are free of responsibility and liability resulting from the use of "
    "this software. Use of this software is at your own risk.\n"
    "\n"
    "We, the SimCenter software designers, are asking you to share your suggestions on how to improve this "
    "educational tool by reporting issues at https://github.com/NHERI-SimCenter/QtPile. \n"
    "We further appreciate your feedback concerning use cases and potential improvements through our web interface "
    "available through the Help->Provide Feedback menu entry.\n"
    "\n"
    "---\n"
    "\n"
    "The SimCenter Pile Group Tool provides a live interface to study the behavior of a pile or pile group "
    "in layered soil.  It allows the user to interactively (and nearly instantly) observe the system's "
    "response to changes of the following parameters:\n"
    "\n"
    " * soil structure (up to 3 layers of variable thickness), soil properties,\n"
    " * position of the ground water table,\n"
    " * pile stiffness, embedment length, connection type to the pile cap,\n"
    " * number of piles in the group (one to three piles), and\n"
    " * the applied horizontal force at the pile cap (push-over analysis).\n"
    "\n"
    "Presented results include\n"
    "\n"
    " * lateral displacement of all piles,\n"
    " * moment and shear diagrams for each pile,\n"
    " * vertical soil stresses\n"
    " * ultimate strength and stiffness parameters for the employed p-y springs.\n"
    "\n"
    "Technical background:\n"
    "\n"
    "The soil-structure system is modeled using the OpenSees finite element platform [1].\n"
    "\n"
    "The pile group is represented as multiple displacement-based elastic beams, "
    "connected by a semi-rigid pile cap. "
    "The connection between piles and pile cap can be modeled as flexible (hinged) or "
    "moment bearing (equal DOF constraint).\n"
    "\n"
    "Soil-structure interaction is represented by appropriate nonlinear p-y spring (t-z spring) "
    "elements for lateral (axial) movement.  Spring properties are computed using Hansen's method [2] "
    "and recommendations by the American Petroleum Institute (API) [3].  "
    "A toe resistance can be added using an isolated T-z spring at the toe of each pile.\n"
    "\n"
    "The Pile Group Tool constructs a structurally and numerically appropriate finite element mesh, "
    "computes spring properties from soil properties, and places spring elements accordingly.  "
    "The actual simulation employs a Newton-Raphson iterative procedure to solve the governing "
    "nonlinear system of equations.\n"
    "\n"
    "The user can quickly explore the effect of changing pile and soil properties to lateral "
    "pile deformation, internal pile moment and shear distributions, overburden stress in the soil, "
    "and spring parameters as obtained through Hansen [2] and API [3] procedures.  "
    "Moreover, the user can easily add and remove piles by the click of a button to observe the "
    "effect of a pile group versus a single pile foundation.\n"
    "\n"
    "References\n"
    " [1] OpenSees. Home page: http://opensees.berkeley.edu/ (last lookup September 25, 2017)\n"
    " [2] Brinch Hansen, J. (1961). “The ultimate resistance of rigid piles against transversal forces.”Bulletin No. 12, Geoteknisk Institute, Copenhagen, 59.\n"
    " [3] American Petroleum Institute (API) (1987). Recommended Practice for Planning, Designing and Constructing Fixed Offshore Platforms. API Recommended Practice 2A(RP-2A), Washington D.C, 17th edition.\n"
    ;
    ui->plainTextEdit->setPlainText(msg);
}

DialogAbout::~DialogAbout()
{
    delete ui;
}
