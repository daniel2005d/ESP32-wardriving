from sqlalchemy import DateTime, create_engine, Column, Integer, String, Double, func
from sqlalchemy.ext.declarative import declarative_base
from sqlalchemy.dialects.postgresql import insert

from sqlalchemy import create_engine
from sqlalchemy.orm import sessionmaker

Base = declarative_base()

class DataBase:
    def __init__(self):
        engine = create_engine('postgresql://postgres:3A9eQAHluSe7@10.0.0.253:5432/gps', 
                       connect_args={'connect_timeout':10, 'application_name':'gps'}, pool_size=20, max_overflow=0)
        self._session = sessionmaker(bind=engine)
    

    def save_network(self, line:str, delimiter='|'):
        point = None
        session = self._session()
        fragments = line.split(delimiter)
        
        if len(fragments) != 6:
            raise Exception(f'Line {line} must have 5 fields.')
        stmt_network = insert(Network).values(
            ssid=fragments[0],
            bssid=fragments[1],
            signal=fragments[2],
            encryption_type=fragments[3],
        ).on_conflict_do_nothing(
            index_elements=['bssid']
        )

        session.execute(stmt_network)

        longitude = fragments[5]
        latitude = fragments[4]
        if float(longitude) < 0: 
            stmt_locations = insert(Location).values(
                bssid = fragments[1],
                latitude = fragments[4],
                longitude = fragments[5]
            ).on_conflict_do_nothing(
                index_elements=['bssid', 'latitude', 'longitude']
            )


            point = f'{fragments[4]},{fragments[5]}'
        
            session.execute(stmt_locations)

        session.commit()
        session.close()

        return point


class Network(Base):
    __tablename__ = 'networks'
    id = Column(Integer, primary_key=True)
    ssid = Column(String, nullable=False)
    bssid = Column(String, nullable=False)
    encryption_type = Column(String, nullable=False)
    signal = Column(Double, nullable=False)

class Location(Base):
    __tablename__ = 'locations'
    id = Column(Integer, primary_key=True)
    bssid = Column(String, nullable=False)
    longitude = Column(Double, nullable=False)
    latitude = Column(Double, nullable=False)
    created = Column(DateTime(timezone=True),
                     nullable=False,
                     server_default=func.now()
                     )


if __name__ == '__main__':
    db = DataBase()
    db.save_network('H3SWifi_A999|99:a9:53:eb:7d:e4|-83|4|4.89|-74.03')