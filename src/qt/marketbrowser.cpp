#include "marketbrowser.h"
#include "ui_marketbrowser.h"
#include "main.h"
#include "wallet.h"
#include "base58.h"
#include "clientmodel.h"
#include "bitcoinrpc.h"
#include <QDesktopServices>

#include <sstream>
#include <string>

using namespace json_spirit;

const QString kBaseUrl = "http://vegacoin.io/dnrusd.php";
const QString kBaseUrl1 = "http://blockchain.info/tobtc?currency=USD&value=1";
const QString kBaseUrl2 = "http://vegacoin.io/dnrmc.php";
const QString kBaseUrl3 = "http://vegacoin.io/dnrbtc.php";

QString bitcoinp = "";
QString vegacoinp = "";
QString dnrmcp = "";
QString dnrbtcp = "";
double bitcoin2;
double vegacoin2;
double dnrmc2;
double dnrbtc2;
QString bitcoing;
QString dnrmarket;
QString dollarg;
int mode=1;
int o = 0;


MarketBrowser::MarketBrowser(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MarketBrowser)
{
    ui->setupUi(this);
    setFixedSize(400, 420);


requests();
QObject::connect(&m_nam, SIGNAL(finished(QNetworkReply*)), this, SLOT(parseNetworkResponse(QNetworkReply*)));
connect(ui->startButton, SIGNAL(pressed()), this, SLOT( requests()));
connect(ui->egal, SIGNAL(pressed()), this, SLOT( update()));

}

void MarketBrowser::update()
{
    QString temps = ui->egals->text();
    double totald = dollarg.toDouble() * temps.toDouble();
    double totaldq = bitcoing.toDouble() * temps.toDouble();
    ui->egald->setText("$ "+QString::number(totald)+" USD or "+QString::number(totaldq)+" BTC");

}

void MarketBrowser::requests()
{
	getRequest(kBaseUrl);
    getRequest(kBaseUrl1);
	getRequest(kBaseUrl2);
	getRequest(kBaseUrl3);
}

void MarketBrowser::getRequest( const QString &urlString )
{
    QUrl url ( urlString );
    QNetworkRequest req ( url );
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json; charset=utf-8");
    m_nam.get(req);
}

void MarketBrowser::parseNetworkResponse(QNetworkReply *finished )
{

    QUrl what = finished->url();

    if ( finished->error() != QNetworkReply::NoError )
    {
        // A communication error has occurred
        emit networkError( finished->error() );
        return;
    }
	
if (what == kBaseUrl) // Vegacoin Price
{

    // QNetworkReply is a QIODevice. So we read from it just like it was a file
    QString vegacoin = finished->readAll();
    vegacoin2 = (vegacoin.toDouble());
    vegacoin = QString::number(vegacoin2);
	
    if(vegacoin > vegacoinp)
    {
        ui->vegacoin->setText("<font color=\"yellow\">$" + vegacoin + "</font>");
    } else if (vegacoin < vegacoinp) {
        ui->vegacoin->setText("<font color=\"red\">$" + vegacoin + "</font>");
        } else {
    ui->vegacoin->setText("$"+vegacoin+" USD");
    }

    vegacoinp = vegacoin;
	dollarg = vegacoin;
}

if (what == kBaseUrl1) // Bitcoin Price
{

    // QNetworkReply is a QIODevice. So we read from it just like it was a file
    QString bitcoin = finished->readAll();
    bitcoin2 = (1 / bitcoin.toDouble());
    bitcoin = QString::number(bitcoin2);
    if(bitcoin > bitcoinp)
    {
        ui->bitcoin->setText("<font color=\"yellow\">$" + bitcoin + " USD</font>");
    } else if (bitcoin < bitcoinp) {
        ui->bitcoin->setText("<font color=\"red\">$" + bitcoin + " USD</font>");
        } else {
    ui->bitcoin->setText("$"+bitcoin+" USD");
    }

    bitcoinp = bitcoin;
}

if (what == kBaseUrl2) // Vegacoin Market Cap
{

    // QNetworkReply is a QIODevice. So we read from it just like it was a file
    QString dnrmc = finished->readAll();
    dnrmc2 = (dnrmc.toDouble());
    dnrmc = QString::number(dnrmc2);
	
    if(dnrmc > dnrmcp)
    {
        ui->dnrmc->setText("<font color=\"yellow\">$" + dnrmc + "</font>");
    } else if (dnrmc < dnrmcp) {
        ui->dnrmc->setText("<font color=\"red\">$" + dnrmc + "</font>");
        } else {
    ui->dnrmc->setText("$"+dnrmc+" USD");
    }

    dnrmcp = dnrmc;
	dnrmarket = dnrmc;
}

if (what == kBaseUrl3) // Vegacoin BTC Price
{

    // QNetworkReply is a QIODevice. So we read from it just like it was a file
    QString dnrbtc = finished->readAll();
    dnrbtc2 = (dnrbtc.toDouble());
    dnrbtc = QString::number(dnrbtc2);
	
    if(dnrbtc > dnrbtcp)
    {
        ui->dnrbtc->setText("<font color=\"yellow\">" + dnrbtc + " BTC</font>");
    } else if (dnrbtc < dnrbtcp) {
        ui->dnrbtc->setText("<font color=\"red\">" + dnrbtc + " BTC</font>");
        } else {
    ui->dnrbtc->setText(dnrbtc+" BTC");
    }

    dnrbtcp = dnrbtc;
	bitcoing = dnrbtc;
}

finished->deleteLater();
}


void MarketBrowser::setModel(ClientModel *model)
{
    this->model = model;
}

MarketBrowser::~MarketBrowser()
{
    delete ui;
}
